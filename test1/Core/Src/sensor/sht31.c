#include "sensor/sht31.h"

#include <stdio.h>
#include "cmsis_os2.h"
#include "main.h"

#if SHT31_USE_HAL_I2C
#include "i2c.h"
#endif

#define SHT31_I2C_ADDR          (0x44U << 1)
#define SHT31_CMD_MEASURE_H    0x24U
#define SHT31_CMD_MEASURE_L    0x00U
#define SHT31_MEASURE_DELAY_MS 20U
#define SHT31_I2C_TIMEOUT_MS   100U

static float last_temperature_c = 25.5f;
static float last_humidity_percent = 60.0f;
static uint8_t sensor_ready = 0U;

static uint8_t sht31_crc8(const uint8_t *data, uint32_t len)
{
    uint8_t crc = 0xFFU;
    uint32_t i;
    uint8_t bit;

    for (i = 0U; i < len; i++) {
        crc ^= data[i];
        for (bit = 0U; bit < 8U; bit++) {
            if ((crc & 0x80U) != 0U) {
                crc = (uint8_t)((crc << 1U) ^ 0x31U);
            } else {
                crc <<= 1U;
            }
        }
    }

    return crc;
}

static uint8_t sht31_update_sample(void)
{
#if SHT31_USE_HAL_I2C
    uint8_t cmd[2] = {SHT31_CMD_MEASURE_H, SHT31_CMD_MEASURE_L};
    uint8_t data[6] = {0};
    uint16_t raw_temp;
    uint16_t raw_hum;

    if (HAL_I2C_Master_Transmit(&hi2c1, SHT31_I2C_ADDR, cmd, sizeof(cmd),
                                SHT31_I2C_TIMEOUT_MS) != HAL_OK) {
        sensor_ready = 0U;
        return 0U;
    }

    osDelay(SHT31_MEASURE_DELAY_MS);

    if (HAL_I2C_Master_Receive(&hi2c1, SHT31_I2C_ADDR, data, sizeof(data),
                               SHT31_I2C_TIMEOUT_MS) != HAL_OK) {
        sensor_ready = 0U;
        return 0U;
    }

    if ((sht31_crc8(&data[0], 2U) != data[2]) ||
        (sht31_crc8(&data[3], 2U) != data[5])) {
        sensor_ready = 0U;
        return 0U;
    }

    raw_temp = (uint16_t)(((uint16_t)data[0] << 8U) | data[1]);
    raw_hum = (uint16_t)(((uint16_t)data[3] << 8U) | data[4]);
    last_temperature_c = -45.0f + (175.0f * (float)raw_temp / 65535.0f);
    last_humidity_percent = 100.0f * (float)raw_hum / 65535.0f;
    sensor_ready = 1U;
    return 1U;
#else
    sensor_ready = 1U;
    return 1U;
#endif
}

void SHT31_Init(void)
{
#if SHT31_USE_HAL_I2C
    HAL_StatusTypeDef probe = HAL_I2C_IsDeviceReady(&hi2c1, SHT31_I2C_ADDR, 3U,
                                                    SHT31_I2C_TIMEOUT_MS);
    if (probe != HAL_OK) {
        printf("[SHT31] not found (I2C1 PB8/PB9, addr=0x44, err=0x%08lX, SCL=%u, SDA=%u)\n",
               (unsigned long)HAL_I2C_GetError(&hi2c1),
               (unsigned int)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8),
               (unsigned int)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9));
        I2C1_DebugScan();
        return;
    }

    if (sht31_update_sample() != 0U) {
        printf("[SHT31] init OK (I2C1 PB8/PB9, addr=0x44)\n");
    } else {
        printf("[SHT31] init failed (I2C1 PB8/PB9, addr=0x44)\n");
    }
#else
    printf("[SHT31] init OK (stub fallback; enable I2C1 PB8/PB9 for real driver)\n");
    (void)sht31_update_sample();
#endif
}

float SHT31_ReadTemperature(void)
{
    (void)sht31_update_sample();
    return last_temperature_c;
}

float SHT31_ReadHumidity(void)
{
    if (sensor_ready == 0U) {
        (void)sht31_update_sample();
    }
    return last_humidity_percent;
}
