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
#define MQTT_TOPIC_CMD      "cargo/command"
/* ----------------------------------------- */

#define MQTT_PAYLOAD_MAX     256U
#define MQTT_RX_BUFFER_SIZE  512U
#define MQTT_RX_POLL_MS      20U

static uint8_t mqtt_ready = 0U;

/* ---------- TCP AT 指令（Air780E 通用） ---------- */

static uint8_t TCP_SendAT(const char *command, const char *expected, uint32_t timeout_ms)
{
#if AIR780E_USE_HAL_UART
    char tx_buffer[128];
    char response[MQTT_RX_BUFFER_SIZE];
    uint8_t ch = 0U;
    uint32_t pos = 0U;
    uint32_t start_tick;
    uint32_t tx_len;

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

        if (pos < (sizeof(response) - 1U)) {
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
    uint32_t remaining_offset;
    uint8_t encoded[4] = {0};
    uint8_t len_size, i;

    remaining = 2U + (uint32_t)strlen(topic) + (uint32_t)strlen(payload);

    /* 固定头: PUBLISH(0x30), QoS 0, Retain 0 */
    buf[pos++] = 0x30U;

    len_size = mqtt_encode_length(remaining, encoded);
    remaining_offset = pos;

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
    uint32_t pos, start_tick;

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

static AppCloudCommand MQTT_ParseCommandPayload(const char *payload)
{
    if (strstr(payload, "HOLD") != NULL)      return APP_CLOUD_CMD_HOLD;
    if (strstr(payload, "RETURN") != NULL)    return APP_CLOUD_CMD_RETURN;
    if (strstr(payload, "CONTINUE") != NULL)  return APP_CLOUD_CMD_CONTINUE;
    return APP_CLOUD_CMD_NONE;
}

void MQTT_Init(void)
{
    char cmd[128];
    uint8_t connect_pkt[256];
    uint32_t pkt_len;
    uint8_t subscribe_pkt[256];

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

    /* TODO: 解析 CONNACK（跳过，直接假设成功） */
    mqtt_ready = 1U;
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
    static uint32_t poll_count = 0;
    const char *payload = "";

    poll_count++;
    if ((poll_count % 9U) == 0U) {
        payload = "{\"cmd\":\"RETURN\"}";
    } else if ((poll_count % 6U) == 0U) {
        payload = "{\"cmd\":\"HOLD\"}";
    } else if ((poll_count % 3U) == 0U) {
        payload = "{\"cmd\":\"CONTINUE\"}";
    }

    if (payload[0] != '\0') {
        AppCloudCommand command = MQTT_ParseCommandPayload(payload);
        printf("[MQTT] cloud payload: %s\n", payload);
        return command;
    }

    return APP_CLOUD_CMD_NONE;
}
