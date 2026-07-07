#include "air780e/mqtt.h"

#include <stdio.h>
#include <string.h>
#include "air780e/air780e.h"

#define MQTT_PAYLOAD_MAX 128U

static uint8_t mqtt_ready = 0U;

static uint8_t MQTT_SendAT(const char *command, const char *expected)
{
    const char *response = "OK";

    printf("[MQTT] >> %s\n", command);
    printf("[MQTT] << %s\n", response);

    return (strstr(response, expected) != NULL) ? 1U : 0U;
}

static void MQTT_BuildTelemetryPayload(const TelemetryData *telemetry, char *payload, uint32_t payload_size)
{
    (void)snprintf(payload,
                   payload_size,
                   "{\"temp\":%d,\"hum\":%d,\"lat\":%d,\"lon\":%d,\"gps_valid\":%u}",
                   (int)telemetry->env.temperature_c,
                   (int)telemetry->env.humidity_percent,
                   (int)telemetry->gps.latitude,
                   (int)telemetry->gps.longitude,
                   telemetry->gps.valid);
}

static AppCloudCommand MQTT_ParseCommandPayload(const char *payload)
{
    if (strstr(payload, "HOLD") != NULL) {
        return APP_CLOUD_CMD_HOLD;
    }

    if (strstr(payload, "RETURN") != NULL) {
        return APP_CLOUD_CMD_RETURN;
    }

    if (strstr(payload, "CONTINUE") != NULL) {
        return APP_CLOUD_CMD_CONTINUE;
    }

    return APP_CLOUD_CMD_NONE;
}

void MQTT_Init(void)
{
    if (Air780E_IsNetworkReady() == 0U) {
        mqtt_ready = 0U;
        printf("[MQTT] init skipped: Air780E network not ready\n");
        return;
    }

    mqtt_ready = MQTT_SendAT("AT+CMQTTSTART", "OK");
    printf("[MQTT] init %s (AT command framework)\n", (mqtt_ready != 0U) ? "OK" : "failed");
}

uint8_t MQTT_PublishTelemetry(const TelemetryData *telemetry)
{
    char payload[MQTT_PAYLOAD_MAX];

    if ((telemetry == NULL) || (Air780E_IsNetworkReady() == 0U) || (mqtt_ready == 0U)) {
        return 0;
    }

    MQTT_BuildTelemetryPayload(telemetry, payload, sizeof(payload));
    printf("[MQTT] publish telemetry %s\n", payload);
    (void)MQTT_SendAT("AT+CMQTTPUB", "OK");

    return 1;
}

AppCloudCommand MQTT_PollCommand(void)
{
    static uint32_t poll_count = 0;
    const char *payload = "";

    poll_count++;
    if ((poll_count % 9U) == 0U) {
        payload = "{\"cmd\":\"RETURN\"}";
    } else if ((poll_count % 6U) == 0U) {
        payload = "{\"cmd\":\"HOLD\"}";
    } else if ((poll_count % 3U) == 0U) {
        payload = "{\"cmd\":\"CONTINUE\"}";
    }

    if (payload[0] != '\0') {
        AppCloudCommand command = MQTT_ParseCommandPayload(payload);

        printf("[MQTT] cloud payload: %s\n", payload);
        return command;
    }

    return APP_CLOUD_CMD_NONE;
}
