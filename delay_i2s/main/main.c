#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "esp_check.h"
#include "sdkconfig.h"

// #include "esp_a2dp_api.h"

// GPIO definitions
#define LED_PIN             GPIO_NUM_2
#define I2S_TX_BCLK_PIN     GPIO_NUM_4
#define I2S_TX_WS_PIN       GPIO_NUM_5
#define I2S_TX_DATA_PIN     GPIO_NUM_18

#define AUDIO_SAMPLE_RATE 44100

#define DATA_IN_BUF_LEN 2048
#define DATA_OUT_BUF_LEN 2048

extern uint32_t runtime_ms;

uint8_t program_run;

static i2s_chan_handle_t tx_ch;

uint8_t *data_in_buf;
uint8_t *data_out_buf;

static void i2s_init(void);
static void mem_init(void);
static void mem_deinit(void);

static void task_toggle_led(void *args)
{
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    bool led_level = false;

    for (;;)
    {
        led_level = !led_level;
        gpio_set_level(LED_PIN, (uint32_t)led_level);
        printf("runtime = %ld\n", runtime_ms);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL);
}

static void task_i2s_write(void *args)
{
    // TODO: Move I2S config to inside task function
    assert(data_out_buf);

    for (;;)
    {
        taskYIELD();
    }

    vTaskDelete(NULL);
}

static void i2s_init(void)
{
    i2s_chan_config_t tx_ch_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    ESP_ERROR_CHECK(i2s_new_channel(&tx_ch_cfg, &tx_ch, NULL));

    i2s_std_config_t tx_std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(AUDIO_SAMPLE_RATE),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = I2S_TX_BCLK_PIN,
            .ws = I2S_TX_WS_PIN,
            .dout = I2S_TX_DATA_PIN,
            .din = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_ch, &tx_std_cfg));
}

static void mem_init(void)
{
    data_in_buf = (uint8_t*)calloc(1, DATA_IN_BUF_LEN);
    data_out_buf = (uint8_t*)calloc(1, DATA_OUT_BUF_LEN);
}

static void mem_deinit(void)
{
    free(data_in_buf);
    free(data_out_buf);
}

void app_main(void)
{
    i2s_init();
    mem_init();

    program_run = 1;

    xTaskCreate(task_toggle_led, "toggle_led", 4096, NULL, 1, NULL);
    xTaskCreate(task_i2s_write, "i2s_write", 4096, NULL, 24, NULL);

    while (program_run == 1)
    {
        taskYIELD();
    }

    mem_deinit();
}