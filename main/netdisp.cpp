#include <network/WifiConnector.hpp>
#include <network/UdpReceiver.hpp>

#include <esp_log.h>
#include <nvs_flash.h>

#include <cstring>

namespace netdisp {

void main() {
  if (!network::WifiConnector::getInstance().connect(
          CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD,
          CONFIG_ESP_MAXIMUM_RETRY)) {
    return;
  }

  network::UdpReceiver Receiver(5432);
  if (!Receiver.isReady()) {
    ESP_LOGE("NetDisp", "Could not setup UDP receiver");
    return;
  }

  while (1) {
    char Buffer[256];
    std::memset(Buffer, 0, sizeof(Buffer));
    int Count = Receiver.recv(Buffer, sizeof(Buffer));
    if (Count < 0) {
      ESP_LOGE("NetDisp", "Failed to receive data");
    }

    ESP_LOGI("NetDisp", "Got message: %s", Buffer);
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