#include <netdisp/Led.hpp>

#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define LOGGER_TAG "LedController"

namespace netdisp {

LedController::LedController(std::vector<int> LP) : LedPins(std::move(LP)) {
  for (auto &LedPin : LedPins) {

    // Check that the LED Pin is valid
    if (!GPIO_IS_VALID_GPIO(LedPin)) {
      ESP_LOGE(LOGGER_TAG, "invalid LED pin %d", LedPin);
      LedPin = -1;
      continue;
    }

    ESP_ERROR_CHECK(gpio_reset_pin(static_cast<gpio_num_t>(LedPin)));
    ESP_ERROR_CHECK(
        gpio_set_direction(static_cast<gpio_num_t>(LedPin), GPIO_MODE_OUTPUT));
  }
}

bool LedController::setLed(unsigned Led, bool State) const {
  if (Led >= LedPins.size() || LedPins.at(Led) == -1) {
    return false;
  }
  return gpio_set_level(static_cast<gpio_num_t>(LedPins.at(Led)), State) ==
         ESP_OK;
}

bool LedController::setLeds(bool State) const {
  for (auto const &LedPin : LedPins) {
    if (gpio_set_level(static_cast<gpio_num_t>(LedPin), State) != ESP_OK) {
      return false;
    }
  }
  return true;
}

bool LedController::blinkLed(unsigned Led, unsigned Times,
                             unsigned DelayMs) const {
  if (!setLed(Led, false)) {
    return false;
  }
  for (unsigned Count = 0; Count < Times; Count++) {
    setLed(Led, true);
    vTaskDelay(DelayMs / portTICK_PERIOD_MS);
    setLed(Led, false);
    vTaskDelay(DelayMs / portTICK_PERIOD_MS);
  }
  return true;
}

} // namespace netdisp