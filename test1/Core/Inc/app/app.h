#ifndef __APP_H
#define __APP_H

#include <stdint.h>

void App_TaskStateMachine(void);
void App_TaskI2CSensors(void);
void App_TaskGPS(void);
void App_Task4GMQTT(void);
void App_TaskFlash(void);
void App_TaskAlarm(void);

void App_SendActivationFromISR(uint16_t event);

#endif
