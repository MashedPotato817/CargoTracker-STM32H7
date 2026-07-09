#include "app/alarm.h"

#include <stdio.h>
#include "cmsis_os2.h"
#include "main.h"

#define ALARM_LED_HEARTBEAT_MS 1000U
#define ALARM_LED_ACTIVE_MS    200U

#define ALARM_DEBOUNCE_MAX     3U
#define ALARM_DEBOUNCE_ON      2U
#define ALARM_DEBOUNCE_OFF     0U

static uint8_t alarm_active = 0;
static uint8_t last_reported = 0xFF;
static uint8_t led_on = 0;
static uint32_t last_toggle_tick = 0;
static uint8_t system_active = 0;
static uint8_t debounce_cnt = 0;

void Alarm_Init(void)
{
    alarm_active = 0;
    last_reported = 0xFF;
    led_on = 0;
    last_toggle_tick = 0;
    HAL_GPIO_WritePin(LD2_YELLOW_GPIO_Port, LD2_YELLOW_Pin, GPIO_PIN_RESET);
#ifdef BUZZER_Pin
    /* 蜂鸣器接法: 3.3V -- 蜂鸣器 -- PC8，HIGH=关 LOW=响 */
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
#endif
    printf("[Alarm] init OK (LD2 yellow heartbeat, buzzer PC8)\n");
}

uint8_t Alarm_FeedSample(const EnvSample *env)
{
    uint8_t over = 0;

    if ((env != NULL) && (env->valid != 0U)) {
        if ((env->temperature_c > ALARM_TEMP_HIGH_C) ||
            (env->humidity_percent > ALARM_HUMID_HIGH_PCT)) {
            over = 1;
        }
    }

    /* 防抖计数器：连续超阈值 +1，正常 -1 */
    if (over) {
        if (debounce_cnt < ALARM_DEBOUNCE_MAX) debounce_cnt++;
    } else {
        if (debounce_cnt > 0U) debounce_cnt--;
    }

    if (!alarm_active && debounce_cnt >= ALARM_DEBOUNCE_ON) {
        printf("[Alarm] debounce triggered (cnt=%u)\n", debounce_cnt);
        return 1;
    }
    if (alarm_active && debounce_cnt <= ALARM_DEBOUNCE_OFF) {
        printf("[Alarm] debounce cleared (cnt=%u)\n", debounce_cnt);
        return 0;
    }

    return alarm_active ? 1U : 0U;
}

void Alarm_SetActive(uint8_t active)
{
    alarm_active = active ? 1U : 0U;
    if (!active) debounce_cnt = 0;
}

void Alarm_SetSystemActive(uint8_t active)
{
    system_active = active ? 1U : 0U;
    if (!system_active) {
        /* 系统休眠 → 关闭所有指示灯和蜂鸣器 */
        HAL_GPIO_WritePin(LD2_YELLOW_GPIO_Port, LD2_YELLOW_Pin, GPIO_PIN_RESET);
#ifdef BUZZER_Pin
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
#endif
        printf("[Alarm] system sleep (LEDs off)\n");
    } else {
        printf("[Alarm] system active (LEDs on)\n");
    }
}

void Alarm_Task(void)
{
    if (!system_active) {
        return;  /* 系统休眠，不做任何 LED/蜂鸣器操作 */
    }

    uint32_t now_tick = osKernelGetTickCount();
    uint32_t interval_ms = alarm_active ? ALARM_LED_ACTIVE_MS : ALARM_LED_HEARTBEAT_MS;

    if ((now_tick - last_toggle_tick) >= interval_ms) {
        last_toggle_tick = now_tick;
        led_on = led_on ? 0U : 1U;
        HAL_GPIO_WritePin(LD2_YELLOW_GPIO_Port,
                          LD2_YELLOW_Pin,
                          led_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
#ifdef BUZZER_Pin
        /* 3.3V--蜂鸣器--PC8: LOW=响 HIGH=关 */
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin,
                          (alarm_active && led_on) ? GPIO_PIN_RESET : GPIO_PIN_SET);
#endif
    }

    if (last_reported != alarm_active) {
        printf("[Alarm] %s\n", alarm_active ? "FAST BLINK" : "SLOW HEARTBEAT");
        last_reported = alarm_active;
    }
}
