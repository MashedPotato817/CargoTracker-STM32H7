#include "gps/gps.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if GPS_USE_HAL_UART
#include "usart.h"
#endif

#define GPS_NMEA_LINE_MAX 96U
#define GPS_UART_CHAR_TIMEOUT_MS 20U
#define GPS_UART_LINE_TIMEOUT_MS 1200U
#define GPS_UART_BAUDRATE 9600U

static GpsLocation last_location = {0.0f, 0.0f, 0U};  /* valid=0 直到真定位 */

static const char *const stub_nmea_sentences[] = {
    "$GNRMC,092204.999,A,3113.8240,N,12128.4220,E,0.00,0.00,070726,,,A*68",
    "$GNGGA,092205.999,3113.8240,N,12128.4220,E,1,08,1.0,10.0,M,0.0,M,,*5B"
};

static uint8_t GPS_ReadLine(char *line, uint32_t line_size)
{
#if GPS_USE_HAL_UART
    uint8_t ch = 0U;
    uint32_t pos = 0U;
    uint32_t start_tick = HAL_GetTick();

    if ((line == NULL) || (line_size == 0U)) {
        return 0U;
    }

    while ((HAL_GetTick() - start_tick) < GPS_UART_LINE_TIMEOUT_MS) {
        if (HAL_UART_Receive(&huart2, &ch, 1U, GPS_UART_CHAR_TIMEOUT_MS) != HAL_OK) {
            continue;
        }

        if (ch == '\r') {
            continue;
        }

        if (ch == '\n') {
            if (pos == 0U) {
                continue;
            }

            line[pos] = '\0';
            return 1U;
        }

        if (pos < (line_size - 1U)) {
            line[pos] = (char)ch;
            pos++;
        }
    }

    line[0] = '\0';
    return 0U;
#else
    static uint32_t stub_index = 0U;
    const char *source = stub_nmea_sentences[stub_index % (sizeof(stub_nmea_sentences) / sizeof(stub_nmea_sentences[0]))];

    stub_index++;
    (void)snprintf(line, line_size, "%s", source);

    return 1U;
#endif
}

static float GPS_ParseCoordinate(const char *value, const char *hemisphere)
{
    float raw = (float)atof(value);
    int degrees = (int)(raw / 100.0f);
    float minutes = raw - ((float)degrees * 100.0f);
    float coordinate = (float)degrees + (minutes / 60.0f);

    if ((hemisphere[0] == 'S') || (hemisphere[0] == 'W')) {
        coordinate = -coordinate;
    }

    return coordinate;
}

static uint8_t GPS_ParseRmc(char *line, GpsLocation *location)
{
    char *fields[13] = {0};
    uint32_t field_count = 0U;
    char *token = strtok(line, ",");

    while ((token != NULL) && (field_count < (sizeof(fields) / sizeof(fields[0])))) {
        fields[field_count] = token;
        field_count++;
        token = strtok(NULL, ",");
    }

    if ((field_count < 7U) || (fields[2][0] != 'A')) {
        return 0U;
    }

    location->latitude = GPS_ParseCoordinate(fields[3], fields[4]);
    location->longitude = GPS_ParseCoordinate(fields[5], fields[6]);
    location->valid = 1U;

    return 1U;
}

static uint8_t GPS_ParseGga(char *line, GpsLocation *location)
{
    char *fields[10] = {0};
    uint32_t field_count = 0U;
    char *token = strtok(line, ",");

    while ((token != NULL) && (field_count < (sizeof(fields) / sizeof(fields[0])))) {
        fields[field_count] = token;
        field_count++;
        token = strtok(NULL, ",");
    }

    if ((field_count < 7U) || (fields[6][0] == '0')) {
        return 0U;
    }

    location->latitude = GPS_ParseCoordinate(fields[2], fields[3]);
    location->longitude = GPS_ParseCoordinate(fields[4], fields[5]);
    location->valid = 1U;

    return 1U;
}

static uint8_t GPS_ParseNmea(const char *line, GpsLocation *location)
{
    char work[GPS_NMEA_LINE_MAX];

    (void)snprintf(work, sizeof(work), "%s", line);

    if ((strncmp(work, "$GNRMC", 6) == 0) || (strncmp(work, "$GPRMC", 6) == 0)) {
        return GPS_ParseRmc(work, location);
    }

    if ((strncmp(work, "$GNGGA", 6) == 0) || (strncmp(work, "$GPGGA", 6) == 0)) {
        return GPS_ParseGga(work, location);
    }

    return 0U;
}

void GPS_Init(void)
{
#if GPS_USE_HAL_UART
    if (huart2.Init.BaudRate != GPS_UART_BAUDRATE) {
        huart2.Init.BaudRate = GPS_UART_BAUDRATE;
        if (HAL_UART_Init(&huart2) != HAL_OK) {
            printf("[GPS] USART2 reinit failed, keep last location fallback\n");
            return;
        }
    }

    printf("[GPS] init OK (HAL UART, ATGM336H on USART2 PD5/PD6)\n");
#else
    printf("[GPS] init OK (stub NMEA, set GPS_USE_HAL_UART=1 after CubeMX enables USART2)\n");
#endif
}

GpsLocation GPS_GetLocation(void)
{
    char line[GPS_NMEA_LINE_MAX];
    GpsLocation location = last_location;
    int tries;

    /* 循环读，直到找到有效的 RMC/GGA 句 */
    for (tries = 0; tries < 10; tries++) {
        if (GPS_ReadLine(line, sizeof(line)) == 0U) {
            break;  /* 超时无数据 */
        }
        if (GPS_ParseNmea(line, &location) != 0U) {
            last_location = location;
            break;
        }
    }

    return location;
}
