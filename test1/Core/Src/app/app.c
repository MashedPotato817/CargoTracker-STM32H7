#include "app/app.h"

#include <stdio.h>
#include "cmsis_os2.h"
#include "main.h"
#include "app/alarm.h"
#include "app/app_types.h"
#include "app/power.h"
#include "app/state_machine.h"
#include "air780e/air780e.h"
#include "air780e/mqtt.h"
#include "flash/w25q128.h"
#include "gps/gps.h"
#include "nfc/pn532.h"
#include "sensor/sht31.h"

extern osMessageQueueId_t queue_activationHandle;
extern osMessageQueueId_t queue_sensor_dataHandle;
extern osMessageQueueId_t queue_cloud_cmdHandle;

#define APP_CLOUD_WAIT_MS 30000U
#define APP_MQTT_POLL_MS 1000U
#define APP_RETURN_BUZZER_MS 15000U
#define APP_ENV_SCALE 10L
#define APP_COORD_SCALE 1000000L

static volatile uint8_t gps_sample_requested = 0;
static volatile uint8_t env_sample_requested = 0;

static void reset_queue(osMessageQueueId_t queue)
{
    if (queue != NULL) {
        (void)osMessageQueueReset(queue);
    }
}

static uint8_t wait_for_sensor_packet(AppSensorPacketType type,
                                      AppSensorPacket *out_packet,
                                      uint32_t timeout_ms)
{
    uint32_t start_tick = osKernelGetTickCount();
    uint32_t packet_addr = 0;

    while ((osKernelGetTickCount() - start_tick) < timeout_ms) {
        if (osMessageQueueGet(queue_sensor_dataHandle, &packet_addr, NULL, 500) == osOK) {
            AppSensorPacket *packet = (AppSensorPacket *)packet_addr;

            if ((packet != NULL) && (packet->type == type)) {
                *out_packet = *packet;
                return 1;
            }
        }
    }

    return 0;
}

static void handle_cloud_command(AppCloudCommand command)
{
    switch (command) {
    case APP_CLOUD_CMD_HOLD:
        printf("[StateMachine] cloud action: HOLD\n");
        Alarm_SetActive(0);
        Alarm_SetLedMode(ALARM_LED_MODE_HOLD_ON);
        break;
    case APP_CLOUD_CMD_RETURN:
        printf("[StateMachine] cloud action: RETURN\n");
        Alarm_SetActive(1);
        Alarm_SetLedMode(ALARM_LED_MODE_FAST_BLINK);
        Alarm_StartBuzzer(APP_RETURN_BUZZER_MS);
        break;
    case APP_CLOUD_CMD_CONTINUE:
        printf("[StateMachine] cloud action: CONTINUE\n");
        Alarm_SetActive(0);
        Alarm_SetLedMode(ALARM_LED_MODE_NORMAL);
        break;
    default:
        printf("[StateMachine] cloud action: NONE\n");
        break;
    }
}

static void request_gps_sample(void)
{
    reset_queue(queue_sensor_dataHandle);
    gps_sample_requested = 1;
}

static void request_env_sample(void)
{
    reset_queue(queue_sensor_dataHandle);
    env_sample_requested = 1;
}

static int32_t scale_fixed1(float value)
{
    float scaled = value * (float)APP_ENV_SCALE;

    return (value >= 0.0f) ? (int32_t)(scaled + 0.5f) : (int32_t)(scaled - 0.5f);
}

static int32_t scale_coordinate(float coordinate)
{
    float scaled = coordinate * (float)APP_COORD_SCALE;

    return (coordinate >= 0.0f) ? (int32_t)(scaled + 0.5f) : (int32_t)(scaled - 0.5f);
}

void App_SendActivationFromISR(uint16_t event)
{
    if (queue_activationHandle != NULL) {
        (void)osMessageQueuePut(queue_activationHandle, &event, 0, 0);
    }
}

void App_TaskStateMachine(void)
{
    uint16_t activation_event = 0;
    uint32_t idle_log_tick = 0U;

    printf("[SM] task started (Air780E OFF, waiting NFC)\n");
    StateMachine_Init();
    printf("[SM] state_machine done\n");
    Alarm_Init();
    printf("[SM] alarm done\n");
    Power_Init();
    printf("[SM] power done\n");

    printf("[StateMachine] waiting first NFC activation...\n");

    for (;;) {
        /* === 等待 NFC 刷卡激活 === */
        if (osMessageQueueGet(queue_activationHandle, &activation_event, NULL, 1000) != osOK) {
            uint32_t now_tick = osKernelGetTickCount();
            if ((now_tick - idle_log_tick) >= 5000U) {
                idle_log_tick = now_tick;
                printf("[StateMachine] waiting NFC...\n");
            }
            continue;
        }

        reset_queue(queue_activationHandle);
        printf("[StateMachine] NFC activated, Air780E ON\n");
        StateMachine_Set(STATE_NFC_ACTIVE);

        Power_Air780E_PowerOn();  /* 开机脉冲 + 等 5s 启动 */
        /* Air780E_Init() 和 MQTT_Init() 内部不再调 PowerOn */
        Air780E_Init();
        MQTT_Init();
        Alarm_SetSystemActive(1);

        /* === Air780E 在线，定时上报（10s/次），NFC 刷卡关机 === */
        for (;;) {
            TelemetryData telemetry = {0};

            /* ---- GPS ---- */
            StateMachine_Set(STATE_GPS_LOCATE);
            reset_queue(queue_sensor_dataHandle);
            request_gps_sample();
            {
                uint32_t t0 = osKernelGetTickCount();
                uint8_t gps_done = 0;
                while ((osKernelGetTickCount() - t0) < 7000U) {
                    uint32_t packet_addr = 0;
                    if (osMessageQueueGet(queue_sensor_dataHandle,
                                          &packet_addr, NULL, 500) == osOK) {
                        AppSensorPacket *p = (AppSensorPacket *)packet_addr;
                        if ((p != NULL) && (p->type == APP_SENSOR_PACKET_GPS)) {
                            telemetry.gps = p->data.gps;
                            gps_done = 1;
                            break;
                        }
                    }
                    /* 随时响应 NFC 关机 */
                    if (osMessageQueueGet(queue_activationHandle,
                                          &activation_event, NULL, 0) == osOK) {
                        printf("[StateMachine] NFC shutdown during GPS\n");
                        goto nfc_shutdown;
                    }
                }
                if (!gps_done) printf("[StateMachine] GPS timeout\n");
            }

            /* ---- SHT31 ---- */
            StateMachine_Set(STATE_ENV_SAMPLE);
            request_env_sample();
            {
                uint32_t t0 = osKernelGetTickCount();
                uint8_t env_done = 0;
                while ((osKernelGetTickCount() - t0) < 5000U) {
                    uint32_t packet_addr = 0;
                    if (osMessageQueueGet(queue_sensor_dataHandle,
                                          &packet_addr, NULL, 500) == osOK) {
                        AppSensorPacket *p = (AppSensorPacket *)packet_addr;
                        if ((p != NULL) && (p->type == APP_SENSOR_PACKET_ENV)) {
                            telemetry.env = p->data.env;
                            env_done = 1;
                            break;
                        }
                    }
                    if (osMessageQueueGet(queue_activationHandle,
                                          &activation_event, NULL, 0) == osOK) {
                        printf("[StateMachine] NFC shutdown during SHT31\n");
                        goto nfc_shutdown;
                    }
                }
                if (!env_done) printf("[StateMachine] SHT31 timeout\n");
            }

            /* ---- 报警（防抖计数） ---- */
            if (Alarm_FeedSample(&telemetry.env)) {
                StateMachine_Set(STATE_ALARM);
                Alarm_SetActive(1);
            } else {
                Alarm_SetActive(0);
            }

            /* ---- MQTT 上传 ---- */
            StateMachine_Set(STATE_UPLOAD);
            reset_queue(queue_cloud_cmdHandle);
            HAL_GPIO_WritePin(LD3_RED_GPIO_Port, LD3_RED_Pin, GPIO_PIN_SET);
            if (MQTT_PublishTelemetry(&telemetry) == 0U) {
                (void)W25Q128_WriteTelemetry(&telemetry);
            }
            HAL_GPIO_WritePin(LD3_RED_GPIO_Port, LD3_RED_Pin, GPIO_PIN_RESET);

            /* ---- 云指令（2s 超时） ---- */
            {
                uint16_t cmd = APP_CLOUD_CMD_NONE;
                uint8_t cmd_received = 0U;
                uint32_t t0 = osKernelGetTickCount();

                StateMachine_Set(STATE_WAIT_CLOUD);
                while ((osKernelGetTickCount() - t0) < APP_CLOUD_WAIT_MS) {
                    if (osMessageQueueGet(queue_cloud_cmdHandle, &cmd, NULL, 500) == osOK) {
                        cmd_received = 1U;
                        break;
                    }
                    if (osMessageQueueGet(queue_activationHandle,
                                          &activation_event, NULL, 0) == osOK) {
                        printf("[StateMachine] NFC shutdown during WAIT_CLOUD\n");
                        goto nfc_shutdown;
                    }
                }

                if (cmd_received != 0U) {
                    handle_cloud_command((AppCloudCommand)cmd);
                } else {
                    handle_cloud_command(APP_CLOUD_CMD_NONE);
                }
            }

            /* 等 10 秒，期间检查 NFC 关机 */
            printf("[StateMachine] next report in 10s (NFC to power off)\n");
            {
                uint32_t t0 = osKernelGetTickCount();
                while ((osKernelGetTickCount() - t0) < 10000U) {
                    if (osMessageQueueGet(queue_activationHandle,
                                          &activation_event, NULL, 500) == osOK) {
                        printf("[StateMachine] NFC received, Air780E OFF\n");
                        goto nfc_shutdown;
                    }
                }
            }
        }

nfc_shutdown:
        reset_queue(queue_activationHandle);
        Alarm_SetSystemActive(0);
        Power_Air780E_PowerOff();
        StateMachine_Set(STATE_RETURN_SLEEP);
        Power_EnterStopStub();
        StateMachine_Set(STATE_SLEEP);
    }
}

void App_TaskI2CSensors(void)
{
    static AppSensorPacket env_packet;
    char uid[24];

    SHT31_Init();
    printf("[I2C Task] SHT31 done, starting PN532...\n");
    PN532_Init();
    printf("[I2C Task] PN532 done\n");

    for (;;) {
        if (PN532_ReadCard(uid, sizeof(uid)) != 0U) {
            uint16_t event = APP_ACTIVATION_NFC;
            (void)osMessageQueuePut(queue_activationHandle, &event, 0, 0);
        }

        if (env_sample_requested != 0U) {
            env_sample_requested = 0;
            env_packet.type = APP_SENSOR_PACKET_ENV;
            env_packet.data.env.temperature_c = SHT31_ReadTemperature();
            env_packet.data.env.humidity_percent = SHT31_ReadHumidity();
            env_packet.data.env.valid = 1;

            uint32_t packet_addr = (uint32_t)&env_packet;
            (void)osMessageQueuePut(queue_sensor_dataHandle, &packet_addr, 0, 0);
            {
                int32_t temp_scaled = scale_fixed1(env_packet.data.env.temperature_c);
                int32_t hum_scaled = scale_fixed1(env_packet.data.env.humidity_percent);
                const char *temp_sign = (temp_scaled < 0) ? "-" : "";
                const char *hum_sign = (hum_scaled < 0) ? "-" : "";
                uint32_t temp_abs = (temp_scaled < 0) ? (uint32_t)(-temp_scaled) : (uint32_t)temp_scaled;
                uint32_t hum_abs = (hum_scaled < 0) ? (uint32_t)(-hum_scaled) : (uint32_t)hum_scaled;

                printf("[I2C] SHT31 sample T=%s%lu.%luC H=%s%lu.%lu%%\n",
                       temp_sign,
                       (unsigned long)(temp_abs / (uint32_t)APP_ENV_SCALE),
                       (unsigned long)(temp_abs % (uint32_t)APP_ENV_SCALE),
                       hum_sign,
                       (unsigned long)(hum_abs / (uint32_t)APP_ENV_SCALE),
                       (unsigned long)(hum_abs % (uint32_t)APP_ENV_SCALE));
            }
        }

        osDelay(500);
    }
}

void App_TaskGPS(void)
{
    static AppSensorPacket gps_packet;

    GPS_Init();

    for (;;) {
        if (gps_sample_requested != 0U) {
            gps_sample_requested = 0;
            gps_packet.type = APP_SENSOR_PACKET_GPS;
            gps_packet.data.gps = GPS_GetLocation();

            uint32_t packet_addr = (uint32_t)&gps_packet;
            (void)osMessageQueuePut(queue_sensor_dataHandle, &packet_addr, 0, 0);
            {
                int32_t lat_scaled = scale_coordinate(gps_packet.data.gps.latitude);
                int32_t lon_scaled = scale_coordinate(gps_packet.data.gps.longitude);
                const char *lat_sign = (lat_scaled < 0) ? "-" : "";
                const char *lon_sign = (lon_scaled < 0) ? "-" : "";
                uint32_t lat_abs = (lat_scaled < 0) ? (uint32_t)(-lat_scaled) : (uint32_t)lat_scaled;
                uint32_t lon_abs = (lon_scaled < 0) ? (uint32_t)(-lon_scaled) : (uint32_t)lon_scaled;

                printf("[GPS] sample lat=%s%lu.%06lu lon=%s%lu.%06lu valid=%u\n",
                       lat_sign,
                       (unsigned long)(lat_abs / (uint32_t)APP_COORD_SCALE),
                       (unsigned long)(lat_abs % (uint32_t)APP_COORD_SCALE),
                       lon_sign,
                       (unsigned long)(lon_abs / (uint32_t)APP_COORD_SCALE),
                       (unsigned long)(lon_abs % (uint32_t)APP_COORD_SCALE),
                       gps_packet.data.gps.valid);
            }
        }

        osDelay(500);
    }
}

void App_Task4GMQTT(void)
{
    printf("[4G Task] idle (waiting NFC to power on Air780E)\n");

    for (;;) {
        AppCloudCommand command = MQTT_PollCommand();

        if (command != APP_CLOUD_CMD_NONE) {
            uint16_t command_value = (uint16_t)command;
            (void)osMessageQueuePut(queue_cloud_cmdHandle, &command_value, 0, 0);
        }

        osDelay(APP_MQTT_POLL_MS);
    }
}

void App_TaskFlash(void)
{
    W25Q128_Init();

    for (;;) {
        W25Q128_Task();
        osDelay(10000);
    }
}

void App_TaskAlarm(void)
{
    for (;;) {
        Alarm_Task();
        osDelay(100);
    }
}
