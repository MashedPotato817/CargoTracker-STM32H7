#include "gps/gps.h"

#include <stdio.h>
#include <string.h>

#if GPS_USE_HAL_UART
#include "usart.h"
#endif

#define GPS_NMEA_LINE_MAX 96U
#define GPS_UART_CHAR_TIMEOUT_MS 20U
#define GPS_UART_LINE_TIMEOUT_MS 1200U
#define GPS_UART_BAUDRATE 9600U
#define GPS_UART_FALLBACK_BAUDRATE 115200U
#define GPS_COORD_SCALE 1000000L
#define GPS_SCAN_SENTENCES 16U

static GpsLocation last_location = {0.0f, 0.0f, 0U};  /* valid=0 直到真定位 */

static const char *const stub_nmea_sentences[] = {
    "$GNRMC,092204.999,A,3113.8240,N,12128.4220,E,0.00,0.00,070726,,,A*72",
    "$GNGGA,092205.999,3113.8240,N,12128.4220,E,1,08,1.0,10.0,M,0.0,M,,*4D"
};

static uint8_t GPS_IsDigit(char ch)
{
    return ((ch >= '0') && (ch <= '9')) ? 1U : 0U;
}

static uint8_t GPS_IsHex(char ch)
{
    return (((ch >= '0') && (ch <= '9')) ||
            ((ch >= 'A') && (ch <= 'F')) ||
            ((ch >= 'a') && (ch <= 'f'))) ? 1U : 0U;
}

static uint8_t GPS_HexValue(char ch)
{
    if ((ch >= '0') && (ch <= '9')) {
        return (uint8_t)(ch - '0');
    }

    if ((ch >= 'A') && (ch <= 'F')) {
        return (uint8_t)(ch - 'A' + 10);
    }

    return (uint8_t)(ch - 'a' + 10);
}

static uint8_t GPS_ChecksumValid(const char *line)
{
    const char *p;
    const char *star = NULL;
    uint8_t checksum = 0U;
    uint8_t received;

    if ((line == NULL) || (line[0] != '$')) {
        return 0U;
    }

    for (p = &line[1]; *p != '\0'; p++) {
        if (*p == '*') {
            star = p;
            break;
        }
        checksum ^= (uint8_t)(*p);
    }

    if ((star == NULL) ||
        (GPS_IsHex(star[1]) == 0U) ||
        (GPS_IsHex(star[2]) == 0U)) {
        return 0U;
    }

    received = (uint8_t)((GPS_HexValue(star[1]) << 4U) | GPS_HexValue(star[2]));

    return (checksum == received) ? 1U : 0U;
}

static uint32_t GPS_SplitFields(char *line, char *fields[], uint32_t max_fields)
{
    char *p = line;
    uint32_t field_count = 0U;

    if ((line == NULL) || (fields == NULL) || (max_fields == 0U)) {
        return 0U;
    }

    while ((field_count < max_fields) && (*p != '\0')) {
        fields[field_count] = p;
        field_count++;

        while ((*p != '\0') && (*p != ',') && (*p != '*')) {
            p++;
        }

        if (*p == ',') {
            *p = '\0';
            p++;
        } else if (*p == '*') {
            *p = '\0';
            break;
        }
    }

    return field_count;
}

#if GPS_USE_HAL_UART
static uint32_t gps_active_baudrate = GPS_UART_BAUDRATE;

static void GPS_ClearUartErrors(void)
{
    __HAL_UART_CLEAR_FLAG(&huart2, UART_CLEAR_FEF | UART_CLEAR_NEF | UART_CLEAR_OREF);
}

static uint8_t GPS_SetBaudRate(uint32_t baudrate)
{
    if (huart2.Init.BaudRate == baudrate) {
        gps_active_baudrate = baudrate;
        return 1U;
    }

    (void)HAL_UART_AbortReceive(&huart2);
    huart2.Init.BaudRate = baudrate;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        printf("[GPS] USART2 reinit %lu failed\n", (unsigned long)baudrate);
        return 0U;
    }

    GPS_ClearUartErrors();
    gps_active_baudrate = baudrate;
    printf("[GPS] USART2 baud=%lu\n", (unsigned long)baudrate);
    return 1U;
}
#endif

static uint8_t GPS_ReadLine(char *line, uint32_t line_size)
{
#if GPS_USE_HAL_UART
    uint8_t ch = 0U;
    uint8_t in_sentence = 0U;
    uint32_t pos = 0U;
    uint32_t start_tick = HAL_GetTick();

    if ((line == NULL) || (line_size == 0U)) {
        return 0U;
    }

    while ((HAL_GetTick() - start_tick) < GPS_UART_LINE_TIMEOUT_MS) {
        if (HAL_UART_Receive(&huart2, &ch, 1U, GPS_UART_CHAR_TIMEOUT_MS) != HAL_OK) {
            GPS_ClearUartErrors();
            continue;
        }

        if (ch == '$') {
            pos = 0U;
            in_sentence = 1U;
        }

        if (in_sentence == 0U) {
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
        } else {
            pos = 0U;
            in_sentence = 0U;
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

static uint8_t GPS_ParseCoordinate(const char *value, const char *hemisphere, float *coordinate)
{
    uint32_t whole = 0U;
    uint32_t fraction = 0U;
    uint32_t fraction_scale = 1U;
    uint32_t degrees;
    uint32_t minute_whole;
    uint32_t minute_scaled;
    uint32_t divisor;
    uint32_t max_degrees;
    int32_t coordinate_scaled;
    uint8_t saw_digit = 0U;

    if ((value == NULL) || (hemisphere == NULL) || (coordinate == NULL) ||
        (value[0] == '\0') || (hemisphere[0] == '\0')) {
        return 0U;
    }

    while (GPS_IsDigit(*value) != 0U) {
        whole = (whole * 10U) + (uint32_t)(*value - '0');
        saw_digit = 1U;
        value++;
    }

    if (*value == '.') {
        value++;
        while ((GPS_IsDigit(*value) != 0U) && (fraction_scale < 1000000U)) {
            fraction = (fraction * 10U) + (uint32_t)(*value - '0');
            fraction_scale *= 10U;
            value++;
        }
    }

    if (saw_digit == 0U) {
        return 0U;
    }

    degrees = whole / 100U;
    minute_whole = whole % 100U;
    minute_scaled = (minute_whole * fraction_scale) + fraction;
    divisor = 60U * fraction_scale;

    if (minute_whole >= 60U) {
        return 0U;
    }

    if ((hemisphere[0] == 'N') || (hemisphere[0] == 'S')) {
        max_degrees = 90U;
    } else if ((hemisphere[0] == 'E') || (hemisphere[0] == 'W')) {
        max_degrees = 180U;
    } else {
        return 0U;
    }

    if ((degrees > max_degrees) ||
        ((degrees == max_degrees) && (minute_scaled != 0U))) {
        return 0U;
    }

    coordinate_scaled = (int32_t)((degrees * (uint32_t)GPS_COORD_SCALE) +
                                  (((uint64_t)minute_scaled * (uint32_t)GPS_COORD_SCALE +
                                   (divisor / 2U)) / divisor));

    if ((hemisphere[0] == 'S') || (hemisphere[0] == 'W')) {
        coordinate_scaled = -coordinate_scaled;
    }

    *coordinate = (float)coordinate_scaled / (float)GPS_COORD_SCALE;
    return 1U;
}

static uint8_t GPS_ParseRmc(char *line, GpsLocation *location)
{
    char *fields[13] = {0};
    uint32_t field_count = GPS_SplitFields(line, fields, (uint32_t)(sizeof(fields) / sizeof(fields[0])));

    if ((field_count < 7U) || (fields[2][0] != 'A')) {
        return 0U;
    }

    if ((GPS_ParseCoordinate(fields[3], fields[4], &location->latitude) == 0U) ||
        (GPS_ParseCoordinate(fields[5], fields[6], &location->longitude) == 0U)) {
        return 0U;
    }
    location->valid = 1U;

    return 1U;
}

static uint8_t GPS_ParseGga(char *line, GpsLocation *location)
{
    char *fields[10] = {0};
    uint32_t field_count = GPS_SplitFields(line, fields, (uint32_t)(sizeof(fields) / sizeof(fields[0])));

    if ((field_count < 7U) || (fields[6][0] == '0')) {
        return 0U;
    }

    if ((GPS_ParseCoordinate(fields[2], fields[3], &location->latitude) == 0U) ||
        (GPS_ParseCoordinate(fields[4], fields[5], &location->longitude) == 0U)) {
        return 0U;
    }
    location->valid = 1U;

    return 1U;
}

static uint8_t GPS_ParseNmea(const char *line, GpsLocation *location)
{
    char work[GPS_NMEA_LINE_MAX];

    (void)snprintf(work, sizeof(work), "%s", line);
    if (GPS_ChecksumValid(work) == 0U) {
        return 0U;
    }

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
    if (GPS_SetBaudRate(GPS_UART_BAUDRATE) == 0U) {
        printf("[GPS] init failed, keep last location fallback\n");
        return;
    }

    printf("[GPS] init OK (HAL UART, USART2 PD5/PD6, auto 9600/115200)\n");
#else
    printf("[GPS] init OK (stub NMEA, set GPS_USE_HAL_UART=1 after CubeMX enables USART2)\n");
#endif
}

GpsLocation GPS_GetLocation(void)
{
    char line[GPS_NMEA_LINE_MAX];
    GpsLocation location = last_location;
    uint8_t baud_index;
    uint32_t baudrates[2] = {
        GPS_UART_BAUDRATE,
        GPS_UART_FALLBACK_BAUDRATE
    };

    location.valid = 0U;

#if GPS_USE_HAL_UART
    if (gps_active_baudrate == GPS_UART_FALLBACK_BAUDRATE) {
        baudrates[0] = GPS_UART_FALLBACK_BAUDRATE;
        baudrates[1] = GPS_UART_BAUDRATE;
    }
#endif

    for (baud_index = 0U; baud_index < (uint8_t)(sizeof(baudrates) / sizeof(baudrates[0])); baud_index++) {
        uint8_t tries;
        uint8_t saw_sentence = 0U;

#if GPS_USE_HAL_UART
        if (GPS_SetBaudRate(baudrates[baud_index]) == 0U) {
            continue;
        }
#endif

        for (tries = 0U; tries < GPS_SCAN_SENTENCES; tries++) {
            if (GPS_ReadLine(line, sizeof(line)) == 0U) {
                break;
            }

            saw_sentence = 1U;
            if (GPS_ParseNmea(line, &location) != 0U) {
                last_location = location;
                printf("[GPS] fix OK baud=%lu\n", (unsigned long)baudrates[baud_index]);
                return location;
            }

            if (tries < 3U) {
                printf("[GPS] NMEA no fix: %s\n", line);
            }
        }

        if (saw_sentence != 0U) {
            printf("[GPS] NMEA received but no valid fix, baud=%lu\n",
                   (unsigned long)baudrates[baud_index]);
            return location;
        }
    }

    printf("[GPS] no NMEA on USART2, check PD6 RX wiring and GPS baud\n");

    return location;
}
