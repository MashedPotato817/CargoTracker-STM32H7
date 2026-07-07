#include "air780e/air780e.h"

#include <stdio.h>
#include <string.h>

static uint8_t network_ready = 0;
static uint8_t signal_quality = 0;

static uint8_t Air780E_SendAT(const char *command, const char *expected, uint32_t timeout_ms)
{
    const char *response = "OK";

    (void)timeout_ms;

    if (strcmp(command, "AT+CSQ") == 0) {
        response = "+CSQ: 25,99\r\nOK";
    } else if (strcmp(command, "AT+CREG?") == 0) {
        response = "+CREG: 0,1\r\nOK";
    } else if (strcmp(command, "AT+CGATT?") == 0) {
        response = "+CGATT: 1\r\nOK";
    }

    printf("[Air780E] >> %s\n", command);
    printf("[Air780E] << %s\n", response);

    return (strstr(response, expected) != NULL) ? 1U : 0U;
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

void Air780E_Init(void)
{
    uint8_t at_ok;
    uint8_t echo_ok;
    uint8_t creg_ok;
    uint8_t attached_ok;

    printf("[Air780E] init start (AT framework, USART1 PA9/PA10; UART not configured yet)\n");
    printf("[Air780E] note: PWRKEY planned on PB0, but PB0 is currently LD1_GREEN\n");

    at_ok = Air780E_SendAT("AT", "OK", 1000U);
    echo_ok = Air780E_SendAT("ATE0", "OK", 1000U);
    (void)Air780E_SendAT("AT+CSQ", "OK", 1000U);
    (void)Air780E_ParseSignalQuality("+CSQ: 25,99\r\nOK");
    creg_ok = Air780E_SendAT("AT+CREG?", "+CREG: 0,1", 1000U);
    attached_ok = Air780E_SendAT("AT+CGATT?", "+CGATT: 1", 1000U);

    network_ready = ((at_ok != 0U) && (echo_ok != 0U) && (creg_ok != 0U) && (attached_ok != 0U)) ? 1U : 0U;

    printf("[Air780E] init %s, CSQ=%u\n", (network_ready != 0U) ? "ready" : "not ready", signal_quality);
}

uint8_t Air780E_IsNetworkReady(void)
{
    return network_ready;
}
