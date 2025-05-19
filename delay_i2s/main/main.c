#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "esp_check.h"
#include "sdkconfig.h"

#define LED_PIN GPIO_NUM_2

extern uint32_t runtime_ms;

static void toggle_led(void *args)
{
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    bool led_level = false;

    while (1) {
        led_level = !led_level;
        gpio_set_level(LED_PIN, (uint32_t)led_level);
        printf("runtime = %ld\n", runtime_ms);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreate(toggle_led, "toggle_led", 4096, NULL, 5, NULL);
    
}