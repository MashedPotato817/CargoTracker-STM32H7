#include "air780e/air780e.h"

#include <stdio.h>
#include <string.h>
#include "app/power.h"

#if AIR780E_USE_HAL_UART
#include "usart.h"
#endif

#define AIR780E_RX_BUFFER_SIZE 160U
#define AIR780E_RX_POLL_MS 20U

static uint8_t network_ready = 0;
static uint8_t signal_quality = 0;
static char at_response[AIR780E_RX_BUFFER_SIZE];

static uint8_t Air780E_SendAT(const char *command, const char *expected, uint32_t timeout_ms)
{
#if AIR780E_USE_HAL_UART
    char tx_buffer[48];
    uint8_t ch = 0U;
    uint32_t pos = 0U;
    uint32_t start_tick;
    uint32_t tx_len;

    tx_len = (uint32_t)snprintf(tx_buffer, sizeof(tx_buffer), "%s\r\n", command);
    if (HAL_UART_Transmit(&huart1, (uint8_t *)tx_buffer, (uint16_t)tx_len, timeout_ms) != HAL_OK) {
        return 0U;
    }

    at_response[0] = '\0';
    start_tick = HAL_GetTick();

    printf("[Air780E] >> %s\n", command);

    while ((HAL_GetTick() - start_tick) < timeout_ms) {
        if (HAL_UART_Receive(&huart1, &ch, 1U, AIR780E_RX_POLL_MS) != HAL_OK) {
            continue;
        }

        if (pos < (sizeof(at_response) - 1U)) {
            at_response[pos] = (char)ch;
            pos++;
            at_response[pos] = '\0';
        }

        if (strstr(at_response, expected) != NULL) {
            printf("[Air780E] << %s\n", at_response);
            return 1U;
        }
    }

    printf("[Air780E] << timeout/no match\n");
    return 0U;
#else
    const char *response = "OK";

    (void)timeout_ms;

    if (strcmp(command, "AT+CSQ") == 0) {
        response = "+CSQ: 25,99\r\nOK";
    } else if (strcmp(command, "AT+CREG?") == 0) {
        response = "+CREG: 0,1\r\nOK";
    } else if (strcmp(command, "AT+CGREG?") == 0) {
        response = "+CGREG: 0,1\r\nOK";
    } else if (strcmp(command, "AT+CEREG?") == 0) {
        response = "+CEREG: 0,1\r\nOK";
    } else if (strcmp(command, "AT+CGATT?") == 0) {
        response = "+CGATT: 1\r\nOK";
    }

    printf("[Air780E] >> %s\n", command);
    printf("[Air780E] << %s\n", response);
    (void)snprintf(at_response, sizeof(at_response), "%s", response);

    return (strstr(response, expected) != NULL) ? 1U : 0U;
#endif
}

static uint8_t Air780E_ParseSignalQuality(const char *response)
{
    const char *prefix = strstr(response, "+CSQ:");
    int csq = 0;

    if (prefix == NULL) {
        return 0U;
    }

    if (sscanf(prefix, "+CSQ: %d", &csq) != 1) {
        return 0U;
    }

    if ((csq < 0) || (csq == 99)) {
        return 0U;
    }

    signal_quality = (uint8_t)csq;
    return 1U;
}

uint8_t Air780E_GetSignalQuality(void)
{
    return signal_quality;
}

static uint8_t Air780E_IsRegistered(const char *response, const char *prefix)
{
    const char *line = strstr(response, prefix);
    const char *fields;
    int mode = 0;
    int status = 0;

    if (line == NULL) {
        return 0U;
    }

    fields = strchr(line, ':');
    if (fields == NULL) {
        return 0U;
    }

    if (sscanf(fields + 1, " %d,%d", &mode, &status) != 2) {
        return 0U;
    }

    (void)mode;

    if ((status == 1) || (status == 5)) {
        return 1U;
    }

    return 0U;
}

void Air780E_Init(void)
{
    uint8_t at_ok;
    uint8_t echo_ok;
    uint8_t creg_ok;
    uint8_t cgreg_ok;
    uint8_t cereg_ok;
    uint8_t registered_ok;
    uint8_t attached_ok;

#if AIR780E_USE_HAL_UART
    printf("[Air780E] init start (HAL UART, USART1 PA9/PA10)\n");
#else
    printf("[Air780E] init start (stub AT, set AIR780E_USE_HAL_UART=1 after CubeMX enables USART1)\n");
#endif
    at_ok = Air780E_SendAT("AT", "OK", 3000U);
    echo_ok = Air780E_SendAT("ATE0", "OK", 1000U);
    (void)Air780E_SendAT("AT+CSQ", "OK", 1000U);
    (void)Air780E_ParseSignalQuality(at_response);
    creg_ok = Air780E_SendAT("AT+CREG?", "OK", 2000U);
    creg_ok = ((creg_ok != 0U) && (Air780E_IsRegistered(at_response, "+CREG:") != 0U)) ? 1U : 0U;
    cgreg_ok = Air780E_SendAT("AT+CGREG?", "OK", 2000U);
    cgreg_ok = ((cgreg_ok != 0U) && (Air780E_IsRegistered(at_response, "+CGREG:") != 0U)) ? 1U : 0U;
    cereg_ok = Air780E_SendAT("AT+CEREG?", "OK", 2000U);
    cereg_ok = ((cereg_ok != 0U) && (Air780E_IsRegistered(at_response, "+CEREG:") != 0U)) ? 1U : 0U;
    registered_ok = ((creg_ok != 0U) || (cgreg_ok != 0U) || (cereg_ok != 0U)) ? 1U : 0U;
    (void)Air780E_SendAT("AT+CGDCONT=1,\"IP\",\"cmnet\"", "OK", 1000U);
    attached_ok = Air780E_SendAT("AT+CGATT?", "+CGATT: 1", 1000U);

    /* AT 可能超时（模块刚启动），以注册状态为准 */
    (void)at_ok;
    network_ready = ((echo_ok != 0U) && (registered_ok != 0U) && (attached_ok != 0U)) ? 1U : 0U;

    printf("[Air780E] init %s, CSQ=%u\n", (network_ready != 0U) ? "ready" : "not ready", signal_quality);
}

uint8_t Air780E_IsNetworkReady(void)
{
    return network_ready;
}
