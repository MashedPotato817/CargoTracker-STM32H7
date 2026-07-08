#ifndef __POWER_H
#define __POWER_H

#include <stdint.h>

void Power_Init(void);
void Power_EnterStopStub(void);
void Power_Air780E_PowerOn(void);
void Power_Air780E_PowerOff(void);
void Power_Air780E_SetPwrKey(uint8_t active);

#endif
