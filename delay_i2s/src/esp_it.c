#include "esp_it.h"

#include <stdint.h>
#include "freertos/FreeRTOS.h"

uint32_t runtime_ms = 0;

void vApplicationTickHook(void)
{
    switch (xPortGetCoreID())
    {
        case 0: // System Core
        {
            break;
        }
        case 1: // App core
        {
            ++runtime_ms;
            break;
        }
        default:
        {
            break;
        }
    }
}