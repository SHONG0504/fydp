#include "esp_it.h"

#include <stdint.h>

uint32_t runtime_ms = 0;

void vApplicationTickHook(void)
{
    ++runtime_ms;
}