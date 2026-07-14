#include "cross_platform_timer.h"

#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <unistd.h>

void sleep_seconds(float seconds)
{
    if (getenv("DSA_TEST_MODE") != NULL)
    {
        return;
    }
    usleep(seconds * 1000000);
}
