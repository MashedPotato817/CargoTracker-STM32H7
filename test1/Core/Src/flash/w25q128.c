#include "flash/w25q128.h"

#include <stdio.h>
#include <string.h>
#include "main.h"

#if W25Q128_USE_HAL_SPI
#include "spi.h"
#endif

#define W25Q128_CMD_WRITE_ENABLE 0x06U
#define W25Q128_CMD_READ_STATUS  0x05U
#define W25Q128_CMD_PAGE_PROGRAM 0x02U
#define W25Q128_CMD_READ_DATA    0x03U
#define W25Q128_CMD_JEDEC_ID     0x9FU
#define W25Q128_STATUS_BUSY      0x01U
#define W25Q128_TIMEOUT_MS       100U
#define W25Q128_TELEM_BASE_ADDR  0x000000U
#define W25Q128_TELEM_SLOT_SIZE  32U
#define W25Q128_TELEM_SLOTS      128U

static uint32_t write_slot = 0U;

typedef struct {
    float temperature_c;
    float humidity_percent;
    float latitude;
    float longitude;
    uint8_t env_valid;
    uint8_t gps_valid;
    uint8_t reserved[6];
    uint32_t magic;
} W25Q128TelemetryRecord;

#if W25Q128_USE_HAL_SPI
static void w25q128_select(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

static void w25q128_deselect(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

static uint8_t w25q128_tx(const uint8_t *data, uint16_t len)
{
    return (HAL_SPI_Transmit(&hspi1, (uint8_t *)data, len,
                             W25Q128_TIMEOUT_MS) == HAL_OK) ? 1U : 0U;
}

static uint8_t w25q128_rx(uint8_t *data, uint16_t len)
{
    return (HAL_SPI_Receive(&hspi1, data, len, W25Q128_TIMEOUT_MS) == HAL_OK)
               ? 1U
               : 0U;
}

static uint8_t w25q128_write_enable(void)
{
    uint8_t cmd = W25Q128_CMD_WRITE_ENABLE;

    w25q128_select();
    if (w25q128_tx(&cmd, 1U) == 0U) {
        w25q128_deselect();
        return 0U;
    }
    w25q128_deselect();
    return 1U;
}

static uint8_t w25q128_wait_ready(void)
{
    uint8_t cmd = W25Q128_CMD_READ_STATUS;
    uint8_t status = W25Q128_STATUS_BUSY;
    uint32_t start = HAL_GetTick();

    while ((HAL_GetTick() - start) < W25Q128_TIMEOUT_MS) {
        w25q128_select();
        if ((w25q128_tx(&cmd, 1U) == 0U) || (w25q128_rx(&status, 1U) == 0U)) {
            w25q128_deselect();
            return 0U;
        }
        w25q128_deselect();

        if ((status & W25Q128_STATUS_BUSY) == 0U) {
            return 1U;
        }
    }

    return 0U;
}

static uint8_t w25q128_page_program(uint32_t addr, const uint8_t *data,
                                    uint16_t len)
{
    uint8_t cmd[4];

    if ((data == NULL) || (len == 0U) || (len > 256U)) {
        return 0U;
    }

    if (w25q128_write_enable() == 0U) {
        return 0U;
    }

    cmd[0] = W25Q128_CMD_PAGE_PROGRAM;
    cmd[1] = (uint8_t)(addr >> 16U);
    cmd[2] = (uint8_t)(addr >> 8U);
    cmd[3] = (uint8_t)addr;

    w25q128_select();
    if ((w25q128_tx(cmd, sizeof(cmd)) == 0U) || (w25q128_tx(data, len) == 0U)) {
        w25q128_deselect();
        return 0U;
    }
    w25q128_deselect();

    return w25q128_wait_ready();
}
#endif

void W25Q128_Init(void)
{
#if W25Q128_USE_HAL_SPI
    uint8_t cmd = W25Q128_CMD_JEDEC_ID;
    uint8_t id[3] = {0};

    w25q128_deselect();
    w25q128_select();
    if ((w25q128_tx(&cmd, 1U) != 0U) && (w25q128_rx(id, sizeof(id)) != 0U)) {
        printf("[W25Q128] JEDEC ID: %02X %02X %02X\n", id[0], id[1], id[2]);
    } else {
        printf("[W25Q128] init failed (SPI1 PA4/PA5/PA6/PA7)\n");
    }
    w25q128_deselect();
#else
    printf("[W25Q128] init OK (stub fallback; enable SPI1 PA4/PA5/PA6/PA7 for real driver)\n");
#endif
}

uint8_t W25Q128_WriteTelemetry(const TelemetryData *telemetry)
{
    W25Q128TelemetryRecord record;
    uint32_t addr;

    if (telemetry == NULL) {
        return 0;
    }

    memset(&record, 0, sizeof(record));
    record.temperature_c = telemetry->env.temperature_c;
    record.humidity_percent = telemetry->env.humidity_percent;
    record.latitude = telemetry->gps.latitude;
    record.longitude = telemetry->gps.longitude;
    record.env_valid = telemetry->env.valid;
    record.gps_valid = telemetry->gps.valid;
    record.magic = 0xA5C35A3CU;

    addr = W25Q128_TELEM_BASE_ADDR + (write_slot * W25Q128_TELEM_SLOT_SIZE);
    write_slot = (write_slot + 1U) % W25Q128_TELEM_SLOTS;

#if W25Q128_USE_HAL_SPI
    if (w25q128_page_program(addr, (const uint8_t *)&record,
                             (uint16_t)sizeof(record)) == 0U) {
        printf("[W25Q128] cache telemetry failed at 0x%06X\n", (unsigned int)addr);
        return 0U;
    }

    printf("[W25Q128] cached telemetry at 0x%06X\n", (unsigned int)addr);
#else
    printf("[W25Q128] cache telemetry stub slot=%u\n", (unsigned int)write_slot);
#endif
    return 1;
}

void W25Q128_Task(void)
{
    printf("[W25Q128] cache task idle\n");
}
