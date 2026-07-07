#ifndef __PN532_H
#define __PN532_H

#include <stdint.h>

/*
 * Set PN532_USE_HAL_I2C to 1 after CubeMX enables I2C1 on PB8/PB9
 * and generates Core/Inc/i2c.h with hi2c1.
 */
#ifndef PN532_USE_HAL_I2C
#define PN532_USE_HAL_I2C 0
#endif

void PN532_Init(void);
uint8_t PN532_ReadCard(char *uid, uint32_t uid_size);

#endif
