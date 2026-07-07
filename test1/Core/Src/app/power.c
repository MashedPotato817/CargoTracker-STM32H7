#include "app/power.h"

#include <stdio.h>

void Power_Init(void)
{
    printf("[Power] init OK (Stop Mode stub)\n");
}

void Power_EnterStopStub(void)
{
    printf("[Power] enter sleep stub, waiting next activation\n");
}
