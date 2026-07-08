#include "air780e/mqtt.h"

#include <stdio.h>
#include <string.h>
#include "air780e/air780e.h"

#if AIR780E_USE_HAL_UART
#include "usart.h"
#endif

/* ---------- MQTT 配置（按需修改） ---------- */
#define MQTT_BROKER_HOST    "broker.emqx.io"
#define MQTT_BROKER_PORT    1883
#define MQTT_CLIENT_ID      "stm32_cargo_001"
#define MQTT_TOPIC_TELEM    "cargo/telemetry"
#define MQTT_TOPIC_CMD      "cargo/cmd"
/* ----------------------------------------- */

#define MQTT_PAYLOAD_MAX     256U
#define MQTT_RX_BUFFER_SIZE  512U
#define MQTT_RX_POLL_MS      20U

static uint8_t mqtt_ready = 0U;

/* ---------- TCP AT 指令（Air780E 通用） ---------- */

static uint8_t TCP_SendATCapture(const char *command,
                                 const char *expected,
                                 uint32_t timeout_ms,
                                 char *response,
                                 uint32_t response_size)
{
#if AIR780E_USE_HAL_UART
    char tx_buffer[128];
    uint8_t ch = 0U;
    uint32_t pos = 0U;
    uint32_t start_tick;
    uint32_t tx_len;

    if ((response == NULL) || (response_size == 0U)) {
        return 0U;
    }

    tx_len = (uint32_t)snprintf(tx_buffer, sizeof(tx_buffer), "%s\r\n", command);
    if (HAL_UART_Transmit(&huart1, (uint8_t *)tx_buffer, (uint16_t)tx_len, 1000U) != HAL_OK) {
        return 0U;
    }

    response[0] = '\0';
    start_tick = HAL_GetTick();

    printf("[MQTT] >> %s\n", command);

    while ((HAL_GetTick() - start_tick) < timeout_ms) {
        if (HAL_UART_Receive(&huart1, &ch, 1U, MQTT_RX_POLL_MS) != HAL_OK) {
            continue;
        }

        if (pos < (response_size - 1U)) {
            response[pos] = (char)ch;
            pos++;
            response[pos] = '\0';
        }

        if (strstr(response, expected) != NULL) {
            printf("[MQTT] << %s\n", response);
            return 1U;
        }
    }

    printf("[MQTT] << timeout\n");
    return 0U;
#else
    printf("[MQTT] >> %s\n", command);
    return (strstr("OK", expected) != NULL) ? 1U : 0U;
#endif
}

static uint8_t TCP_SendAT(const char *command, const char *expected, uint32_t timeout_ms)
{
    char response[MQTT_RX_BUFFER_SIZE];

    return TCP_SendATCapture(command, expected, timeout_ms, response, sizeof(response));
}

/* ---------- 二进制 MQTT 包构造 ---------- */

static uint8_t mqtt_encode_length(uint32_t len, uint8_t *out)
{
    uint8_t pos = 0U;

    do {
        uint8_t byte = (uint8_t)(len & 0x7FU);
        len >>= 7U;
        if (len > 0U) { byte |= 0x80U; }
        out[pos++] = byte;
    } while (len > 0U);

    return pos;
}

static uint8_t mqtt_write_str(uint8_t *buf, uint32_t offset, const char *str)
{
    uint16_t len = (uint16_t)strlen(str);

    buf[offset++] = (uint8_t)(len >> 8U);
    buf[offset++] = (uint8_t)(len & 0xFFU);
    (void)memcpy(&buf[offset], str, len);
    return (uint8_t)(2U + len);
}

static uint32_t mqtt_build_connect(uint8_t *buf)
{
    uint32_t pos = 0U;
    uint32_t remaining_offset;
    uint32_t remaining;
    uint8_t len_size;

    /* 固定头: CONNECT(0x10) */
    buf[pos++] = 0x10U;

    /* 剩余长度占位 */
    remaining_offset = pos;
    pos++;

    /* 协议名 "MQTT" */
    pos += mqtt_write_str(buf, pos, "MQTT");

    /* 协议级别 MQTT 3.1.1 = 4 */
    buf[pos++] = 0x04U;

    /* 连接标志: Clean Session */
    buf[pos++] = 0x02U;

    /* Keep Alive: 60s */
    buf[pos++] = 0x00U;
    buf[pos++] = 0x3CU;

    /* 客户端 ID */
    pos += mqtt_write_str(buf, pos, MQTT_CLIENT_ID);

    /* 回填剩余长度 */
    remaining = (pos - remaining_offset - 1U);
    /* shift everything right if we need more than 1 byte for length */
    if (remaining > 127U) {
        (void)memmove(&buf[remaining_offset + 2U],
                      &buf[remaining_offset + 1U],
                      pos - remaining_offset - 1U);
        len_size = mqtt_encode_length(remaining, &buf[remaining_offset]);
        (void)len_size; /* should be 2 */
        pos++;
    } else {
        len_size = mqtt_encode_length(remaining, &buf[remaining_offset]);
        (void)len_size;
    }

    return pos;
}

static uint32_t mqtt_build_publish(uint8_t *buf, const char *topic, const char *payload)
{
    uint32_t pos = 0U;
    uint32_t remaining;
    uint8_t encoded[4] = {0};
    uint8_t len_size, i;

    remaining = 2U + (uint32_t)strlen(topic) + (uint32_t)strlen(payload);

    /* 固定头: PUBLISH(0x30), QoS 0, Retain 0 */
    buf[pos++] = 0x30U;

    len_size = mqtt_encode_length(remaining, encoded);

    /* 如果需要多字节，预留空间 */
    if (len_size > 1U) {
        for (i = 0U; i < len_size; i++) {
            buf[pos++] = encoded[i];
        }
    } else {
        buf[pos++] = encoded[0];
    }

    /* 主题 */
    pos += mqtt_write_str(buf, pos, topic);

    /* 载荷 */
    (void)memcpy(&buf[pos], payload, strlen(payload));
    pos += (uint32_t)strlen(payload);

    /* 修正：如果预留空间过多/过少重新调整 */
    if (len_size == 1U && remaining > 127U) {
        /* 这种情况很少发生，忽略 */
    }

    return pos;
}

/* ---------- 二进制发送（走 CIPSEND） ---------- */

static uint8_t TCP_SendBinary(uint8_t *data, uint32_t len)
{
#if AIR780E_USE_HAL_UART
    char cmd[32];
    uint8_t ch;
    uint32_t start_tick;

    /* CIPSEND */
    (void)snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%lu", (unsigned long)len);
    if (TCP_SendAT(cmd, ">", 2000U) == 0U) {
        printf("[MQTT] CIPSEND failed\n");
        return 0U;
    }

    /* 发送原始二进制 */
    HAL_Delay(50);
    if (HAL_UART_Transmit(&huart1, data, (uint16_t)len, 5000U) != HAL_OK) {
        return 0U;
    }

    /* 等 SEND OK */
    start_tick = HAL_GetTick();
    while ((HAL_GetTick() - start_tick) < 5000U) {
        if (HAL_UART_Receive(&huart1, &ch, 1U, 20U) == HAL_OK) {
            /* just drain */
        }
        /* 注意：模块返回 "SEND OK" 但我们不解析它 */
        if ((HAL_GetTick() - start_tick) > 200U) {
            break;  /* 给 200ms 收确认 */
        }
    }

    return 1U;
#else
    (void)data;
    (void)len;
    return 1U;
#endif
}

/* ---------- MQTT API ---------- */

static void MQTT_BuildTelemetryPayload(const TelemetryData *telemetry, char *payload, uint32_t payload_size)
{
    (void)snprintf(payload,
                   payload_size,
                   "{\"temp\":%d,\"hum\":%d,\"lat\":%d,\"lon\":%d,\"gps_valid\":%u}",
                   (int)telemetry->env.temperature_c,
                   (int)telemetry->env.humidity_percent,
                   (int)telemetry->gps.latitude,
                   (int)telemetry->gps.longitude,
                   telemetry->gps.valid);
}

static uint8_t MQTT_BufferContains(const uint8_t *buf, uint32_t len, const char *needle)
{
    uint32_t needle_len = (uint32_t)strlen(needle);
    uint32_t i;

    if ((buf == NULL) || (needle_len == 0U) || (len < needle_len)) {
        return 0U;
    }

    for (i = 0U; i <= (len - needle_len); i++) {
        if (memcmp(&buf[i], needle, needle_len) == 0) {
            return 1U;
        }
    }

    return 0U;
}

static AppCloudCommand MQTT_ParseCommandBytes(const uint8_t *buf, uint32_t len)
{
    if ((MQTT_BufferContains(buf, len, "HOLD") != 0U) ||
        (MQTT_BufferContains(buf, len, "484F4C44") != 0U)) {
        return APP_CLOUD_CMD_HOLD;
    }
    if ((MQTT_BufferContains(buf, len, "RETURN") != 0U) ||
        (MQTT_BufferContains(buf, len, "52455455524E") != 0U)) {
        return APP_CLOUD_CMD_RETURN;
    }
    if ((MQTT_BufferContains(buf, len, "CONTINUE") != 0U) ||
        (MQTT_BufferContains(buf, len, "434F4E54494E5545") != 0U)) {
        return APP_CLOUD_CMD_CONTINUE;
    }

    return APP_CLOUD_CMD_NONE;
}

static AppCloudCommand MQTT_ParsePublishBuffer(const uint8_t *buf, uint32_t len)
{
    uint32_t i;

    if ((buf == NULL) || (len == 0U)) {
        return APP_CLOUD_CMD_NONE;
    }

    for (i = 0U; i < len; i++) {
        uint8_t header = buf[i];

        if ((header & 0xF0U) == 0x30U) {
            uint32_t offset = i + 1U;
            uint32_t remaining_len = 0U;
            uint32_t multiplier = 1U;
            uint8_t encoded = 0U;
            uint8_t encoded_count = 0U;

            do {
                if (offset >= len) {
                    break;
                }

                encoded = buf[offset++];
                remaining_len += (uint32_t)(encoded & 0x7FU) * multiplier;
                multiplier *= 128U;
                encoded_count++;
            } while (((encoded & 0x80U) != 0U) && (encoded_count < 4U));

            if (((encoded & 0x80U) != 0U) ||
                ((offset + remaining_len) > len) ||
                ((offset + 2U) > len)) {
                continue;
            }

            {
                uint32_t packet_end = offset + remaining_len;
                uint16_t topic_len = ((uint16_t)buf[offset] << 8U) | (uint16_t)buf[offset + 1U];
                uint8_t qos = (uint8_t)((header & 0x06U) >> 1U);
                uint32_t payload_offset;

                offset += 2U;
                if ((offset + topic_len) > packet_end) {
                    continue;
                }

                payload_offset = offset + topic_len;
                if (qos > 0U) {
                    payload_offset += 2U;
                }

                if (payload_offset <= packet_end) {
                    uint32_t payload_len = packet_end - payload_offset;
                    AppCloudCommand command = MQTT_ParseCommandBytes(&buf[payload_offset], payload_len);

                    if (command != APP_CLOUD_CMD_NONE) {
                        char payload[MQTT_PAYLOAD_MAX];
                        uint32_t copy_len = (payload_len < (sizeof(payload) - 1U)) ?
                                            payload_len : (sizeof(payload) - 1U);
                        uint32_t j;

                        for (j = 0U; j < copy_len; j++) {
                            uint8_t c = buf[payload_offset + j];
                            payload[j] = ((c >= 32U) && (c <= 126U)) ? (char)c : '.';
                        }
                        payload[copy_len] = '\0';
                        printf("[MQTT] cloud cmd received: %s\n", payload);
                        return command;
                    }
                }
            }
        }
    }

    {
        AppCloudCommand command = MQTT_ParseCommandBytes(buf, len);

        if (command != APP_CLOUD_CMD_NONE) {
            printf("[MQTT] cloud cmd received (raw)\n");
        }

        return command;
    }
}

#if AIR780E_USE_HAL_UART
static uint32_t MQTT_ReadUartBytes(uint8_t *buf, uint32_t max_len, uint32_t timeout_ms)
{
    uint32_t pos = 0U;
    uint32_t start_tick = HAL_GetTick();
    uint32_t last_rx_tick = start_tick;

    if ((buf == NULL) || (max_len == 0U)) {
        return 0U;
    }

    while (((HAL_GetTick() - start_tick) < timeout_ms) && (pos < max_len)) {
        if (HAL_UART_Receive(&huart1, &buf[pos], 1U, MQTT_RX_POLL_MS) == HAL_OK) {
            pos++;
            last_rx_tick = HAL_GetTick();
        } else if ((pos > 0U) && ((HAL_GetTick() - last_rx_tick) >= 50U)) {
            break;
        }
    }

    return pos;
}
#endif

void MQTT_Init(void)
{
    char cmd[128];
    uint8_t connect_pkt[256];
    uint32_t pkt_len;

    if (Air780E_IsNetworkReady() == 0U) {
        mqtt_ready = 0U;
        printf("[MQTT] init skipped: Air780E network not ready\n");
        return;
    }

#if AIR780E_USE_HAL_UART
    printf("[MQTT] init: Air780E TCP MQTT stack\n");

    /* Step 1: 开 TCP 连接 */
    (void)snprintf(cmd, sizeof(cmd),
                   "AT+CIPSTART=\"TCP\",\"%s\",%u",
                   MQTT_BROKER_HOST, MQTT_BROKER_PORT);
    if (TCP_SendAT(cmd, "CONNECT", 10000U) == 0U) {
        printf("[MQTT] TCP connect failed\n");
        mqtt_ready = 0U;
        return;
    }

    /* Step 2: 发 MQTT CONNECT 包 */
    pkt_len = mqtt_build_connect(connect_pkt);
    printf("[MQTT] sending MQTT CONNECT (%lu bytes)\n", (unsigned long)pkt_len);
    if (TCP_SendBinary(connect_pkt, pkt_len) == 0U) {
        printf("[MQTT] MQTT CONNECT send failed\n");
        mqtt_ready = 0U;
        return;
    }

    /* Step 3: 读 CONNACK（broker 回 4 字节，清空以免干扰后续 CIPSEND） */
    {
        uint8_t drain[64] = {0};
        uint32_t drain_pos = 0;
        uint8_t ch;
        uint32_t t = HAL_GetTick();

        printf("[MQTT] waiting CONNACK...\n");
        while ((HAL_GetTick() - t) < 2000U) {
            if (HAL_UART_Receive(&huart1, &ch, 1U, 10U) == HAL_OK) {
                if (drain_pos < sizeof(drain) - 1U) {
                    drain[drain_pos++] = ch;
                    drain[drain_pos] = '\0';
                }
            }
            /* 收到 4+ 字节 CONNACK 就停 */
            if (drain_pos >= 4U) break;
        }
        printf("[MQTT] CONNACK drain: %lu bytes\n", (unsigned long)drain_pos);
    }

    mqtt_ready = 1U;

    /* Step 4: 订阅云端指令 topic */
    {
        const uint8_t sub[] = {
            0x82, 0x0E,
            0x00, 0x01,
            0x00, 0x09,
            'c', 'a', 'r', 'g', 'o', '/', 'c', 'm', 'd',
            0x00
        };
        if (TCP_SendBinary((uint8_t *)sub, sizeof(sub)) == 0U) {
            mqtt_ready = 0U;
            printf("[MQTT] subscribe %s failed\n", MQTT_TOPIC_CMD);
            return;
        }
        printf("[MQTT] subscribed %s\n", MQTT_TOPIC_CMD);
    }

    printf("[MQTT] init OK (TCP MQTT, %s:%u)\n", MQTT_BROKER_HOST, MQTT_BROKER_PORT);
#else
    mqtt_ready = 1U;
    printf("[MQTT] init OK (stub)\n");
#endif
}

uint8_t MQTT_PublishTelemetry(const TelemetryData *telemetry)
{
    char payload[MQTT_PAYLOAD_MAX];
    uint8_t pkt[512];
    uint32_t pkt_len;

    if ((telemetry == NULL) || (Air780E_IsNetworkReady() == 0U) || (mqtt_ready == 0U)) {
        return 0;
    }

    MQTT_BuildTelemetryPayload(telemetry, payload, sizeof(payload));

#if AIR780E_USE_HAL_UART
    pkt_len = mqtt_build_publish(pkt, MQTT_TOPIC_TELEM, payload);
    printf("[MQTT] publish %s (%lu bytes)\n", payload, (unsigned long)pkt_len);
    return TCP_SendBinary(pkt, pkt_len);
#else
    printf("[MQTT] publish telemetry %s\n", payload);
    return 1;
#endif
}

AppCloudCommand MQTT_PollCommand(void)
{
#if AIR780E_USE_HAL_UART
    uint8_t rx[MQTT_RX_BUFFER_SIZE] = {0};
    char response[MQTT_RX_BUFFER_SIZE] = {0};
    uint32_t rx_len;
    AppCloudCommand command;

    if ((Air780E_IsNetworkReady() == 0U) || (mqtt_ready == 0U)) {
        return APP_CLOUD_CMD_NONE;
    }

    rx_len = MQTT_ReadUartBytes(rx, sizeof(rx), 250U);
    command = MQTT_ParsePublishBuffer(rx, rx_len);
    if (command != APP_CLOUD_CMD_NONE) {
        return command;
    }

    if (TCP_SendATCapture("AT+CIPRXGET=2,256",
                          "+CIPRXGET:",
                          2000U,
                          response,
                          sizeof(response)) != 0U) {
        command = MQTT_ParsePublishBuffer((const uint8_t *)response, (uint32_t)strlen(response));
        if (command != APP_CLOUD_CMD_NONE) {
            return command;
        }

        rx_len = MQTT_ReadUartBytes(rx, sizeof(rx), 500U);
        command = MQTT_ParsePublishBuffer(rx, rx_len);
        if (command != APP_CLOUD_CMD_NONE) {
            return command;
        }
    }

    return APP_CLOUD_CMD_NONE;
#else
    return APP_CLOUD_CMD_NONE;
#endif
}
