#ifndef __SHT31_H
#define __SHT31_H

/*
 * Set SHT31_USE_HAL_I2C to 1 after CubeMX enables I2C1 on PB8/PB9
 * and generates Core/Inc/i2c.h with hi2c1.
 */
#ifndef SHT31_USE_HAL_I2C
#define SHT31_USE_HAL_I2C 1
#endif

void SHT31_Init(void);
float SHT31_ReadTemperature(void);
float SHT31_ReadHumidity(void);

#endif
