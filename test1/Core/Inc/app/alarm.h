#ifndef __ALARM_H
#define __ALARM_H

#include <stdint.h>
#include "app/app_types.h"

#define ALARM_TEMP_HIGH_C      30.0f
#define ALARM_HUMID_HIGH_PCT   75.0f

void Alarm_Init(void);
uint8_t Alarm_CheckEnv(const EnvSample *env);
void Alarm_SetActive(uint8_t active);
void Alarm_Task(void);

#endif
