#include <network/WifiConnector.hpp>

#include "nvs_flash.h"

namespace netdisp {

void main() {
  if (!network::WifiConnector::getInstance().connect(
          CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD,
          CONFIG_ESP_MAXIMUM_RETRY)) {
    return;
  }
}

} // namespace netdisp

extern "C" {

void app_main(void) {
  // Initialize NVS
  // TODO move this
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  netdisp::main();
}
}