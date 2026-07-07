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
        break;
    case APP_CLOUD_CMD_RETURN:
        printf("[StateMachine] cloud action: RETURN\n");
        break;
    case APP_CLOUD_CMD_CONTINUE:
        printf("[StateMachine] cloud action: CONTINUE\n");
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

void App_SendActivationFromISR(uint16_t event)
{
    if (queue_activationHandle != NULL) {
        (void)osMessageQueuePut(queue_activationHandle, &event, 0, 0);
    }
}

void App_TaskStateMachine(void)
{
    uint16_t activation_event = 0;
    uint16_t cloud_value = APP_CLOUD_CMD_NONE;

    StateMachine_Init();
    Alarm_Init();
    Power_Init();

    for (;;) {
        HAL_GPIO_TogglePin(LD1_GREEN_GPIO_Port, LD1_GREEN_Pin);

        if (osMessageQueueGet(queue_activationHandle, &activation_event, NULL, 1000) != osOK) {
            continue;
        }

        reset_queue(queue_activationHandle);
        reset_queue(queue_sensor_dataHandle);

        TelemetryData telemetry = {0};
        AppSensorPacket packet = {0};

        printf("[StateMachine] activation event=%u\n", activation_event);
        StateMachine_Set(STATE_NFC_ACTIVE);

        StateMachine_Set(STATE_GPS_LOCATE);
        request_gps_sample();
        if (wait_for_sensor_packet(APP_SENSOR_PACKET_GPS, &packet, 7000) != 0U) {
            telemetry.gps = packet.data.gps;
        } else {
            printf("[StateMachine] GPS timeout\n");
        }

        StateMachine_Set(STATE_ENV_SAMPLE);
        request_env_sample();
        if (wait_for_sensor_packet(APP_SENSOR_PACKET_ENV, &packet, 5000) != 0U) {
            telemetry.env = packet.data.env;
        } else {
            printf("[StateMachine] SHT31 timeout\n");
        }

        if (Alarm_CheckEnv(&telemetry.env) != 0U) {
            StateMachine_Set(STATE_ALARM);
            Alarm_SetActive(1);
        } else {
            Alarm_SetActive(0);
        }

        StateMachine_Set(STATE_UPLOAD);
        HAL_GPIO_WritePin(LD3_RED_GPIO_Port, LD3_RED_Pin, GPIO_PIN_SET);
        if (MQTT_PublishTelemetry(&telemetry) == 0U) {
            (void)W25Q128_WriteTelemetry(&telemetry);
        }
        HAL_GPIO_WritePin(LD3_RED_GPIO_Port, LD3_RED_Pin, GPIO_PIN_RESET);

        reset_queue(queue_cloud_cmdHandle);
        StateMachine_Set(STATE_WAIT_CLOUD);
        cloud_value = APP_CLOUD_CMD_NONE;
        if (osMessageQueueGet(queue_cloud_cmdHandle, &cloud_value, NULL, 5000) == osOK) {
            handle_cloud_command((AppCloudCommand)cloud_value);
        } else {
            handle_cloud_command(APP_CLOUD_CMD_NONE);
        }

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
    PN532_Init();

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
            printf("[I2C] SHT31 sample T=%dC H=%d%%\n",
                   (int)env_packet.data.env.temperature_c,
                   (int)env_packet.data.env.humidity_percent);
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
            printf("[GPS] sample lat=%d lon=%d\n",
                   (int)gps_packet.data.gps.latitude,
                   (int)gps_packet.data.gps.longitude);
        }

        osDelay(500);
    }
}

void App_Task4GMQTT(void)
{
    Air780E_Init();
    MQTT_Init();

    for (;;) {
        AppCloudCommand command = MQTT_PollCommand();

        if (command != APP_CLOUD_CMD_NONE) {
            uint16_t command_value = (uint16_t)command;
            (void)osMessageQueuePut(queue_cloud_cmdHandle, &command_value, 0, 0);
        }

        osDelay(4000);
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
        osDelay(500);
    }
}
