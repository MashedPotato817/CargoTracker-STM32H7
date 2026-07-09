#ifndef __ALARM_H
#define __ALARM_H

#include <stdint.h>
#include "app/app_types.h"

#define ALARM_TEMP_HIGH_C      30.0f
#define ALARM_HUMID_HIGH_PCT   75.0f

typedef enum {
    ALARM_LED_MODE_NORMAL = 0,
    ALARM_LED_MODE_HOLD_ON,
    ALARM_LED_MODE_FAST_BLINK
} AlarmLedMode;

void Alarm_Init(void);
uint8_t Alarm_FeedSample(const EnvSample *env);
void Alarm_SetActive(uint8_t active);
void Alarm_SetLedMode(AlarmLedMode mode);
void Alarm_StartBuzzer(uint32_t duration_ms);
void Alarm_SetSystemActive(uint8_t active);
void Alarm_Task(void);

#endif
