/* M5StackC - Example

   See README.md file to get detailed usage of this example.
*/
#include <stdio.h>
#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"
#include "m5stickc.h"
#include "sdkconfig.h"
#include "wire.h"
#include "AXP192.h"
#include "driver/i2s.h"
#include "esp_event.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "bt_main.h"
#include "wifi_main.h"

static uint8_t strbuff[50];
static bool button_a_flag = false;
static bool button_b_flag = false;

#define PIN_CLK 0
#define PIN_DATA 34
#define READ_LEN (2 * 200)

#define BULLET_INITIAL_NUM 20

uint8_t i2s_read_buff[READ_LEN] = {0};
int16_t adc_data_buff[200] = {};
int16_t *padc_16bit_buffer = NULL;
static int gun_num = 0;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void buttonEvent(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data)
{
    if ((base == button_a.esp_event_base) && (id == BUTTON_PRESSED_EVENT))
    {
        //TFT_print("button A", CENTER, 60);
        button_a_flag = true;
    }

    if ((base == button_b.esp_event_base) && (id == BUTTON_PRESSED_EVENT))
    {
        //TFT_print("button B", CENTER, 60);
        button_b_flag = true;
    }
}

void i2sInit()
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
        .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 128,
    };

    i2s_pin_config_t pin_config;
    pin_config.bck_io_num = I2S_PIN_NO_CHANGE;
    pin_config.ws_io_num = PIN_CLK;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num = PIN_DATA;

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

void stickc_test_task()
{
    int bt_state_pre = -1;
    while (true)
    {
        if (button_a_flag)
        {
            button_a_flag = false;
            if (gun_num == 10)
            {
                gun_num = 0;
            }
            else
            {
                gun_num++;
            }
            bt_set_gun_num(gun_num);
            char print_buf[32];
            sprintf(print_buf, "gun_num: %d", gun_num);
            TFT_print(print_buf, CENTER, 10);
        }
        if (button_b_flag)
        {
            button_b_flag = false;
            bt_set_bullet_num(BULLET_INITIAL_NUM);
        }
        char bullet_num_print_buf[64];
        sprintf(bullet_num_print_buf, "bullet_num: %d", bt_get_bullet_num());
        TFT_print(bullet_num_print_buf, CENTER, 20);
        char state_buf[32];
        int bt_state = bt_get_main_state();
        if(bt_state != bt_state_pre){
            switch(bt_state){
                case 0:
                    sprintf(state_buf, "IDLE(%d)", bt_state);
                    break;
                case 1:
                    sprintf(state_buf, "DISCOVERING(%d)", bt_state);
                    break;
                case 2:
                    sprintf(state_buf, "DISCOVERED(%d)", bt_state);
                    break;
                case 3:
                    sprintf(state_buf, "UNCONNECTED(%d)", bt_state);
                    break;
                case 4:
                    sprintf(state_buf, "CONNECTING(%d)", bt_state);
                    break;
                case 5:
                    sprintf(state_buf, "CONNECTED(%d)", bt_state);
                    break;
                case 6:
                    sprintf(state_buf, "DISCONNECTING(%d)", bt_state);
                    break;
                default:
                    sprintf(state_buf, "UNKNOWN(%d)", bt_state);
                    break;
            }
            TFT_print("                                ", 0, 40);
            TFT_print(state_buf, CENTER, 40);
            bt_state_pre = bt_state;
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    M5Init();
    i2sInit();

    esp_event_handler_register_with(event_loop, BUTTON_A_EVENT_BASE, BUTTON_PRESSED_EVENT, buttonEvent, NULL);
    esp_event_handler_register_with(event_loop, BUTTON_B_EVENT_BASE, BUTTON_PRESSED_EVENT, buttonEvent, NULL);

    font_rotate = 0;
    text_wrap = 0;
    font_transparent = 0;
    font_forceFixed = 0;
    gray_scale = 0;
    TFT_setGammaCurve(DEFAULT_GAMMA_CURVE);
    TFT_setRotation(LANDSCAPE);
    TFT_setFont(SMALL_FONT, NULL);
    TFT_resetclipwin();

    TFT_print(">>>M5 StickC<<<", CENTER, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // Initialize NVS.
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_sta();

    bt_set_bullet_num(BULLET_INITIAL_NUM);
    bt_main(NULL);

    xTaskCreate(stickc_test_task, "stickc_test_task", 1024 * 2, (void *)0, 10, NULL);
}
