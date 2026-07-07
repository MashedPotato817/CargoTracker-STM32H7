#ifndef __GPS_H
#define __GPS_H

#include "app/app_types.h"

#ifndef GPS_USE_HAL_UART
#define GPS_USE_HAL_UART 1
#endif

void GPS_Init(void);
GpsLocation GPS_GetLocation(void);

#endif
