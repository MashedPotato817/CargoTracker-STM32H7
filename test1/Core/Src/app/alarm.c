#include "app/alarm.h"

#include <stdio.h>
#include "main.h"

static uint8_t alarm_active = 0;
static uint8_t last_reported = 0xFF;

void Alarm_Init(void)
{
    alarm_active = 0;
    last_reported = 0xFF;
    HAL_GPIO_WritePin(LD2_YELLOW_GPIO_Port, LD2_YELLOW_Pin, GPIO_PIN_RESET);
    printf("[Alarm] init OK (LD2 yellow, buzzer stub)\n");
}

uint8_t Alarm_CheckEnv(const EnvSample *env)
{
    if ((env == NULL) || (env->valid == 0U)) {
        return 0;
    }

    return (env->temperature_c > ALARM_TEMP_HIGH_C) ||
           (env->humidity_percent > ALARM_HUMID_HIGH_PCT);
}

void Alarm_SetActive(uint8_t active)
{
    alarm_active = active ? 1U : 0U;
}

void Alarm_Task(void)
{
    HAL_GPIO_WritePin(LD2_YELLOW_GPIO_Port,
                      LD2_YELLOW_Pin,
                      alarm_active ? GPIO_PIN_SET : GPIO_PIN_RESET);

    if (last_reported != alarm_active) {
        printf("[Alarm] %s\n", alarm_active ? "ON" : "OFF");
        last_reported = alarm_active;
    }
}
