#ifndef __MQTT_H
#define __MQTT_H

#include <stdint.h>
#include "app/app_types.h"

void MQTT_Init(void);
uint8_t MQTT_PublishTelemetry(const TelemetryData *telemetry);
AppCloudCommand MQTT_PollCommand(void);

#endif
