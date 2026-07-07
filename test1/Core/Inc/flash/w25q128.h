#ifndef __W25Q128_H
#define __W25Q128_H

#include <stdint.h>
#include "app/app_types.h"

/*
 * Set W25Q128_USE_HAL_SPI to 1 after CubeMX enables SPI1 on PA5/PA6/PA7
 * and configures PA4 as a GPIO output chip-select pin.
 */
#ifndef W25Q128_USE_HAL_SPI
#define W25Q128_USE_HAL_SPI 0
#endif

void W25Q128_Init(void);
uint8_t W25Q128_WriteTelemetry(const TelemetryData *telemetry);
void W25Q128_Task(void);

#endif
