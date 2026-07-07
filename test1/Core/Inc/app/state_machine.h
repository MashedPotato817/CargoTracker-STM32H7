#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

typedef enum {
    STATE_SLEEP = 0,
    STATE_NFC_ACTIVE,
    STATE_GPS_LOCATE,
    STATE_ENV_SAMPLE,
    STATE_UPLOAD,
    STATE_WAIT_CLOUD,
    STATE_ALARM,
    STATE_RETURN_SLEEP
} SystemState;

void StateMachine_Init(void);
void StateMachine_Set(SystemState state);
SystemState StateMachine_Get(void);
const char *StateMachine_Name(SystemState state);

#endif
