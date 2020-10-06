#ifndef __DIGITAL_SWITCH_HPP__
#define __DIGITAL_SWITCH_HPP__

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

class DigitalSwitch
{
public:
  DigitalSwitch(gpio_num_t pin, unsigned int chatter_count, unsigned int chatter_ms, bool is_rise = false)
  : _pin(pin), _chatter_count(chatter_count), _chatter_ms(chatter_ms), _is_rise(is_rise)
  {
    // set pin config
    gpio_config_t io_conf{};
    // bit mask of the pins
    io_conf.pin_bit_mask = 1ULL << pin;
    // disable interrupt
    io_conf.intr_type = static_cast<gpio_int_type_t>(GPIO_PIN_INTR_DISABLE);
    // set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    if(is_rise){
      // enable pull-up mode
      io_conf.pull_down_en = static_cast<gpio_pulldown_t>(1);
    }else{
      // enable pull-down mode
      io_conf.pull_up_en = static_cast<gpio_pullup_t>(1);
    }
    gpio_config(&io_conf);
  }
  ~DigitalSwitch() = default;
  bool isOn(void)
  {
    if(_pin == GPIO_NUM_NC) return false;

    int detect_level = 0;
    if(_is_rise){
      detect_level = 1;
    }

    for(int detect_count = 0; detect_count <= _chatter_count; detect_count++){
      ESP_LOGD(LOG_TAG, "detecting...");
      if(gpio_get_level(_pin) != detect_level){
        return false;
      }
      vTaskDelay(_chatter_ms / portTICK_PERIOD_MS);
    }

    return true;
  }

private:
  gpio_num_t _pin = GPIO_NUM_NC;
  unsigned int _chatter_count = 0;
  unsigned int _chatter_ms = 0;
  bool _is_rise = false;
  const char* LOG_TAG = "DigitalSwitch";
};


#endif // __DIGITAL_SWITCH_HPP__