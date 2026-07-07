#include "app/alarm.h"

#include <stdio.h>
#include "cmsis_os2.h"
#include "main.h"

#define ALARM_LED_HEARTBEAT_MS 1000U
#define ALARM_LED_ACTIVE_MS    200U

static uint8_t alarm_active = 0;
static uint8_t last_reported = 0xFF;
static uint8_t led_on = 0;
static uint32_t last_toggle_tick = 0;

void Alarm_Init(void)
{
    alarm_active = 0;
    last_reported = 0xFF;
    led_on = 0;
    last_toggle_tick = 0;
    HAL_GPIO_WritePin(LD2_YELLOW_GPIO_Port, LD2_YELLOW_Pin, GPIO_PIN_RESET);
    printf("[Alarm] init OK (LD2 yellow heartbeat/alarm, buzzer stub)\n");
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
    uint32_t now_tick = osKernelGetTickCount();
    uint32_t interval_ms = alarm_active ? ALARM_LED_ACTIVE_MS : ALARM_LED_HEARTBEAT_MS;

    if ((now_tick - last_toggle_tick) >= interval_ms) {
        last_toggle_tick = now_tick;
        led_on = led_on ? 0U : 1U;
        HAL_GPIO_WritePin(LD2_YELLOW_GPIO_Port,
                          LD2_YELLOW_Pin,
                          led_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }

    if (last_reported != alarm_active) {
        printf("[Alarm] %s\n", alarm_active ? "FAST BLINK" : "SLOW HEARTBEAT");
        last_reported = alarm_active;
    }
}
