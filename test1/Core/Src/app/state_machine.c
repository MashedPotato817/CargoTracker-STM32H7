#include "app/state_machine.h"

#include <stdio.h>

static SystemState current_state = STATE_SLEEP;

void StateMachine_Init(void)
{
    current_state = STATE_SLEEP;
    printf("[StateMachine] init -> %s\n", StateMachine_Name(current_state));
}

void StateMachine_Set(SystemState state)
{
    if (current_state != state) {
        printf("[StateMachine] %s -> %s\n",
               StateMachine_Name(current_state),
               StateMachine_Name(state));
        current_state = state;
    }
}

SystemState StateMachine_Get(void)
{
    return current_state;
}

const char *StateMachine_Name(SystemState state)
{
    switch (state) {
    case STATE_SLEEP:
        return "SLEEP";
    case STATE_NFC_ACTIVE:
        return "NFC_ACTIVE";
    case STATE_GPS_LOCATE:
        return "GPS_LOCATE";
    case STATE_ENV_SAMPLE:
        return "ENV_SAMPLE";
    case STATE_UPLOAD:
        return "UPLOAD";
    case STATE_WAIT_CLOUD:
        return "WAIT_CLOUD";
    case STATE_ALARM:
        return "ALARM";
    case STATE_RETURN_SLEEP:
        return "RETURN_SLEEP";
    default:
        return "UNKNOWN";
    }
}
