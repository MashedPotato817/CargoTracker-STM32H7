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
static AlarmLedMode led_mode = ALARM_LED_MODE_NORMAL;
static AlarmLedMode last_reported_mode = (AlarmLedMode)0xFF;
static uint8_t buzzer_timed_active = 0U;
static uint32_t buzzer_stop_tick = 0U;

void Alarm_Init(void)
{
    alarm_active = 0;
    last_reported = 0xFF;
    led_on = 0;
    last_toggle_tick = 0;
    led_mode = ALARM_LED_MODE_NORMAL;
    last_reported_mode = (AlarmLedMode)0xFF;
    buzzer_timed_active = 0U;
    buzzer_stop_tick = 0U;
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

void Alarm_SetLedMode(AlarmLedMode mode)
{
    led_mode = mode;
    last_toggle_tick = 0U;

    if (mode == ALARM_LED_MODE_HOLD_ON) {
        buzzer_timed_active = 0U;
        led_on = 1U;
        HAL_GPIO_WritePin(LD2_YELLOW_GPIO_Port, LD2_YELLOW_Pin, GPIO_PIN_SET);
#ifdef BUZZER_Pin
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
#endif
    } else if (mode == ALARM_LED_MODE_NORMAL) {
        buzzer_timed_active = 0U;
#ifdef BUZZER_Pin
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
#endif
    }
}

void Alarm_StartBuzzer(uint32_t duration_ms)
{
#ifdef BUZZER_Pin
    if (duration_ms == 0U) {
        buzzer_timed_active = 0U;
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
        return;
    }

    buzzer_timed_active = 1U;
    buzzer_stop_tick = osKernelGetTickCount() + duration_ms;
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
#else
    (void)duration_ms;
#endif
}

void Alarm_SetSystemActive(uint8_t active)
{
    system_active = active ? 1U : 0U;
    if (!system_active) {
        alarm_active = 0U;
        debounce_cnt = 0U;
        led_mode = ALARM_LED_MODE_NORMAL;
        led_on = 0U;
        buzzer_timed_active = 0U;
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
    uint8_t fast_blink = ((alarm_active != 0U) || (led_mode == ALARM_LED_MODE_FAST_BLINK)) ? 1U : 0U;
    uint32_t interval_ms = fast_blink ? ALARM_LED_ACTIVE_MS : ALARM_LED_HEARTBEAT_MS;

#ifdef BUZZER_Pin
    if ((buzzer_timed_active != 0U) && ((int32_t)(now_tick - buzzer_stop_tick) >= 0)) {
        buzzer_timed_active = 0U;
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
    }
#endif

    if (led_mode == ALARM_LED_MODE_HOLD_ON) {
        if (led_on == 0U) {
            led_on = 1U;
            HAL_GPIO_WritePin(LD2_YELLOW_GPIO_Port, LD2_YELLOW_Pin, GPIO_PIN_SET);
        }
        if (last_reported_mode != led_mode) {
            printf("[Alarm] LED2 HOLD ON\n");
            last_reported_mode = led_mode;
        }
        return;
    }

    if ((now_tick - last_toggle_tick) >= interval_ms) {
        last_toggle_tick = now_tick;
        led_on = led_on ? 0U : 1U;
        HAL_GPIO_WritePin(LD2_YELLOW_GPIO_Port,
                          LD2_YELLOW_Pin,
                          led_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
#ifdef BUZZER_Pin
        /* 3.3V--蜂鸣器--PC8: LOW=响 HIGH=关 */
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin,
                          ((buzzer_timed_active != 0U) ||
                           ((alarm_active != 0U) && (led_mode == ALARM_LED_MODE_NORMAL) && led_on)) ?
                          GPIO_PIN_RESET : GPIO_PIN_SET);
#endif
    }

    if ((last_reported != fast_blink) || (last_reported_mode != led_mode)) {
        printf("[Alarm] %s\n", fast_blink ? "FAST BLINK" : "SLOW HEARTBEAT");
        last_reported = fast_blink;
        last_reported_mode = led_mode;
    }
}
