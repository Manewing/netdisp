#include <netdisp/Led.hpp>
#include <netdisp/Parser.hpp>
#include <netdisp/View.hpp>
#include <network/UdpReceiver.hpp>
#include <network/WifiConnector.hpp>

#include <netdisp/LcdgfxDisplay.hpp>

#include <esp_log.h>
#include <nvs_flash.h>

#include <cstring>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#ifndef CONFIG_NETDISP_PORT
#define NETDISP_PORT 5432
#else
#define NETDISP_PORT CONFIG_NETDISP_PORT
#endif
#define NETDISP_PORT_STR STR(NETDISP_PORT)

namespace netdisp {

void main() {
  LcdgfxDisplayController DispCtrl;

  netdisp::LedController LedCtrl({17, 16});
  LedCtrl.setLed(0, true);
  LedCtrl.setLed(1, true);

  auto &WifiConn = network::WifiConnector::getInstance();
  if (!WifiConn.connect(CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD,
                        CONFIG_ESP_MAXIMUM_RETRY)) {
    return;
  }

  constexpr int Port = 5432;
  network::UdpReceiver Receiver(Port);
  if (!Receiver.isReady()) {
    ESP_LOGE("NetDisp", "Could not setup UDP receiver");
    return;
  }

  auto DefaultView = std::make_shared<TextView>(
      "~*Ready*\n\n~" + WifiConn.getIpAddrStr() + "\n~" NETDISP_PORT_STR);
  netdisp::ViewController ViewCtrl(DefaultView, 10);

  LedCtrl.setLed(0, false);
  LedCtrl.setLed(1, false);

  netdisp::Context Ctx{LedCtrl, ViewCtrl};

  ViewCtrl.show(DispCtrl);

  while (1) {
    uint8_t Buffer[256];
    std::memset(Buffer, 0, sizeof(Buffer));
    int Count = Receiver.recv(Buffer, sizeof(Buffer));
    if (Count < 0) {
      ESP_LOGE("NetDisp", "Failed to receive data");
      continue;
    }
    // FIXME create hexdump from buffer
    ESP_LOGI("NetDisp", "Got message: %s", Buffer);

    netdisp::Parser Parser(Buffer, Count);
    if (auto Cmd = Parser.parse()) {
      Cmd->execute(Ctx);
    } else {
      ESP_LOGW("NestDisp", "Failed to parse message command");
    }

    ViewCtrl.show(DispCtrl);
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