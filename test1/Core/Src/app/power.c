#include "app/power.h"

#include <stdio.h>
#include "cmsis_os2.h"
#include "main.h"

#define AIR780E_PWRKEY_PULSE_MS 1500U
#define AIR780E_BOOT_WAIT_MS    5000U

void Power_Init(void)
{
    Power_Air780E_SetPwrKey(0);
    printf("[Power] init OK (Sleep Mode ready)\n");
}

void Power_EnterStopStub(void)
{
    printf("[Power] enter Sleep Mode (CPU off, wake on IRQ)\n");
    HAL_SuspendTick();
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    HAL_ResumeTick();
    printf("[Power] wake from Sleep\n");
}

void Power_Air780E_PowerOn(void)
{
    printf("[Power] Air780E power-on (PWRKEY low 1.5s)\n");
    Power_Air780E_SetPwrKey(1);
    osDelay(AIR780E_PWRKEY_PULSE_MS);
    Power_Air780E_SetPwrKey(0);
    printf("[Power] Air780E boot wait 5s\n");
    osDelay(AIR780E_BOOT_WAIT_MS);
}

void Power_Air780E_PowerOff(void)
{
#if defined(AIR780E_PWRKEY_GPIO_Port) && defined(AIR780E_PWRKEY_Pin)
    printf("[Power] Air780E power-off (PWRKEY low 1.5s)\n");
    Power_Air780E_SetPwrKey(1);
    osDelay(AIR780E_PWRKEY_PULSE_MS);
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
