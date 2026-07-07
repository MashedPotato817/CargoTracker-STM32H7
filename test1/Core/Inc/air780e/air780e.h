#ifndef __AIR780E_H
#define __AIR780E_H

#include <stdint.h>

#ifndef AIR780E_USE_HAL_UART
#define AIR780E_USE_HAL_UART 1
#endif

void Air780E_Init(void);
uint8_t Air780E_IsNetworkReady(void);

#endif
