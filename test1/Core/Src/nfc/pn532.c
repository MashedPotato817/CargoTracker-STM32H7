#include "nfc/pn532.h"

#include <stdio.h>
#include "main.h"

#if PN532_USE_HAL_I2C
#include "i2c.h"
#endif

#define PN532_I2C_ADDR        (0x24U << 1)
#define PN532_HOST_TO_PN532   0xD4U
#define PN532_PN532_TO_HOST   0xD5U
#define PN532_CMD_SAM_CONFIG  0x14U
#define PN532_CMD_INLIST      0x4AU
#define PN532_STATUS_READY    0x01U
#define PN532_TIMEOUT_MS      100U
#define PN532_READY_RETRIES   20U
#define PN532_FRAME_MAX       32U

#if PN532_USE_HAL_I2C
static uint8_t pn532_read_ready_frame(uint8_t *frame, uint16_t frame_size)
{
    uint8_t retry;

    for (retry = 0U; retry < PN532_READY_RETRIES; retry++) {
        if ((HAL_I2C_Master_Receive(&hi2c1, PN532_I2C_ADDR, frame, frame_size,
                                    PN532_TIMEOUT_MS) == HAL_OK) &&
            (frame[0] == PN532_STATUS_READY)) {
            return 1U;
        }

        HAL_Delay(5U);
    }

    return 0U;
}

static uint8_t pn532_write_command(const uint8_t *cmd, uint8_t cmd_len)
{
    uint8_t frame[PN532_FRAME_MAX] = {0};
    uint8_t len = (uint8_t)(cmd_len + 1U);
    uint8_t sum = (uint8_t)(PN532_HOST_TO_PN532);
    uint8_t i;
    uint8_t pos = 0U;

    if ((cmd == NULL) || ((uint32_t)cmd_len + 8U > PN532_FRAME_MAX)) {
        return 0U;
    }

    frame[pos++] = 0x00U;
    frame[pos++] = 0x00U;
    frame[pos++] = 0xFFU;
    frame[pos++] = len;
    frame[pos++] = (uint8_t)(0x100U - len);
    frame[pos++] = PN532_HOST_TO_PN532;

    for (i = 0U; i < cmd_len; i++) {
        frame[pos++] = cmd[i];
        sum = (uint8_t)(sum + cmd[i]);
    }

    frame[pos++] = (uint8_t)(0x100U - sum);
    frame[pos++] = 0x00U;

    return (HAL_I2C_Master_Transmit(&hi2c1, PN532_I2C_ADDR, frame, pos,
                                    PN532_TIMEOUT_MS) == HAL_OK) ? 1U : 0U;
}

static uint8_t pn532_read_ack(void)
{
    uint8_t frame[7] = {0};

    if (pn532_read_ready_frame(frame, sizeof(frame)) == 0U) {
        return 0U;
    }

    return ((frame[1] == 0x00U) && (frame[2] == 0x00U) &&
            (frame[3] == 0xFFU) && (frame[4] == 0x00U) &&
            (frame[5] == 0xFFU) && (frame[6] == 0x00U)) ? 1U : 0U;
}

static uint8_t pn532_send_command(const uint8_t *cmd, uint8_t cmd_len)
{
    return ((pn532_write_command(cmd, cmd_len) != 0U) &&
            (pn532_read_ack() != 0U)) ? 1U : 0U;
}

static uint8_t pn532_read_response(uint8_t expected_cmd, uint8_t *payload,
                                   uint8_t payload_size, uint8_t *payload_len)
{
    uint8_t frame[PN532_FRAME_MAX] = {0};
    uint8_t len;
    uint8_t i;

    if (pn532_read_ready_frame(frame, sizeof(frame)) == 0U) {
        return 0U;
    }

    if ((frame[0] != PN532_STATUS_READY) || (frame[1] != 0x00U) ||
        (frame[2] != 0x00U) || (frame[3] != 0xFFU)) {
        return 0U;
    }

    len = frame[4];
    if ((uint8_t)(len + frame[5]) != 0U) {
        return 0U;
    }

    if ((len < 2U) || (frame[6] != PN532_PN532_TO_HOST) ||
        (frame[7] != (uint8_t)(expected_cmd + 1U))) {
        return 0U;
    }

    *payload_len = (uint8_t)(len - 2U);
    if (*payload_len > payload_size) {
        *payload_len = payload_size;
    }

    for (i = 0U; i < *payload_len; i++) {
        payload[i] = frame[8U + i];
    }

    return 1U;
}
#endif

void PN532_Init(void)
{
#if PN532_USE_HAL_I2C
    uint8_t cmd[] = {PN532_CMD_SAM_CONFIG, 0x01U, 0x14U, 0x01U};
    uint8_t payload[4] = {0};
    uint8_t payload_len = 0U;

    if (HAL_I2C_IsDeviceReady(&hi2c1, PN532_I2C_ADDR, 3U,
                              PN532_TIMEOUT_MS) != HAL_OK) {
        printf("[PN532] not found (I2C1 PB8/PB9, addr=0x24, err=0x%08lX, SCL=%u, SDA=%u)\n",
               (unsigned long)HAL_I2C_GetError(&hi2c1),
               (unsigned int)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8),
               (unsigned int)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9));
        I2C1_DebugScan();
        return;
    }

    if ((pn532_send_command(cmd, sizeof(cmd)) != 0U) &&
        (pn532_read_response(PN532_CMD_SAM_CONFIG, payload, sizeof(payload),
                             &payload_len) != 0U)) {
        printf("[PN532] init OK (I2C1 PB8/PB9, addr=0x24)\n");
    } else {
        printf("[PN532] init failed (I2C1 PB8/PB9, addr=0x24)\n");
    }
#else
    printf("[PN532] init OK (stub fallback; enable I2C1 PB8/PB9 for real driver)\n");
#endif
}

uint8_t PN532_ReadCard(char *uid, uint32_t uid_size)
{
#if PN532_USE_HAL_I2C
    uint8_t cmd[] = {PN532_CMD_INLIST, 0x01U, 0x00U};
    uint8_t payload[20] = {0};
    uint8_t payload_len = 0U;
    uint8_t uid_len;
    uint8_t i;
    int written = 0;

    if ((uid == NULL) || (uid_size == 0U)) {
        return 0U;
    }

    if ((pn532_send_command(cmd, sizeof(cmd)) == 0U) ||
        (pn532_read_response(PN532_CMD_INLIST, payload, sizeof(payload),
                             &payload_len) == 0U)) {
        return 0U;
    }

    if ((payload_len < 7U) || (payload[0] == 0U)) {
        return 0U;
    }

    uid_len = payload[5];
    if ((uint32_t)uid_len + 6U > payload_len) {
        return 0U;
    }

    uid[0] = '\0';
    for (i = 0U; i < uid_len; i++) {
        written += snprintf(&uid[written], uid_size - (uint32_t)written,
                            "%02X", payload[6U + i]);
        if ((uint32_t)written >= uid_size) {
            uid[uid_size - 1U] = '\0';
            break;
        }
    }

    printf("[PN532] card detected: %s\n", uid);
    return 1U;
#else
    static uint32_t poll_count = 0;

    poll_count++;
    if ((poll_count % 5U) != 1U) {
        return 0;
    }

    if ((uid != NULL) && (uid_size > 0U)) {
        (void)snprintf(uid, uid_size, "MOCK-UID-001");
    }

    printf("[PN532] card detected: MOCK-UID-001\n");
    return 1;
#endif
}
