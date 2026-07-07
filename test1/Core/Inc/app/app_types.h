#ifndef __APP_TYPES_H
#define __APP_TYPES_H

#include <stdint.h>

typedef enum {
    APP_ACTIVATION_NFC = 1,
    APP_ACTIVATION_BUTTON = 2
} AppActivationEvent;

typedef enum {
    APP_SENSOR_PACKET_ENV = 1,
    APP_SENSOR_PACKET_GPS = 2
} AppSensorPacketType;

typedef enum {
    APP_CLOUD_CMD_NONE = 0,
    APP_CLOUD_CMD_HOLD = 1,
    APP_CLOUD_CMD_RETURN = 2,
    APP_CLOUD_CMD_CONTINUE = 3
} AppCloudCommand;

typedef struct {
    float latitude;
    float longitude;
    uint8_t valid;
} GpsLocation;

typedef struct {
    float temperature_c;
    float humidity_percent;
    uint8_t valid;
} EnvSample;

typedef struct {
    AppSensorPacketType type;
    union {
        EnvSample env;
        GpsLocation gps;
    } data;
} AppSensorPacket;

typedef struct {
    EnvSample env;
    GpsLocation gps;
} TelemetryData;

#endif
