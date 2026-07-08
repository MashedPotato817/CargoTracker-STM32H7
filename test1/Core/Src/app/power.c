#include "app/power.h"

#include <stdio.h>
#include "main.h"

void Power_Init(void)
{
    Power_Air780E_SetPwrKey(0);
#if defined(AIR780E_PWRKEY_GPIO_Port) && defined(AIR780E_PWRKEY_Pin)
    printf("[Power] init OK (Stop Mode stub, Air780E PWRKEY GPIO ready)\n");
#else
    printf("[Power] init OK (Stop Mode stub, Air780E PWRKEY not configured)\n");
#endif
}

void Power_EnterStopStub(void)
{
    printf("[Power] enter sleep stub, waiting next activation\n");
}

void Power_Air780E_PowerOn(void)
{
    Power_Air780E_SetPwrKey(0);
    printf("[Power] Air780E PWRKEY released for VBAT auto-start\n");
}

void Power_Air780E_PowerOff(void)
{
#if defined(AIR780E_PWRKEY_GPIO_Port) && defined(AIR780E_PWRKEY_Pin)
    printf("[Power] Air780E power-off (PWRKEY low 1.5s)\n");
    Power_Air780E_SetPwrKey(1);
    HAL_Delay(1500);
    Power_Air780E_SetPwrKey(0);
#endif
}

void Power_Air780E_SetPwrKey(uint8_t active)
{
#if defined(AIR780E_PWRKEY_GPIO_Port) && defined(AIR780E_PWRKEY_Pin)
    HAL_GPIO_WritePin(AIR780E_PWRKEY_GPIO_Port,
                      AIR780E_PWRKEY_Pin,
                      (active != 0U) ? GPIO_PIN_RESET : GPIO_PIN_SET);
#else
    (void)active;
#endif
}
