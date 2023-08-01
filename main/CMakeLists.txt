
idf_component_register(
  SRCS
    "src/ByteStream.cpp"
    "src/CanvasView.cpp"
    "src/Command.cpp"
    "src/CommandBase.cpp"
    "src/CommandBuilder.cpp"
    "src/CompositeView.cpp"
    "src/Display.cpp"
    "src/ExtraFonts.c"
    "src/LcdgfxDisplay.cpp"
    "src/Led.cpp"
    "src/Notification.cpp"
    "src/Parser.cpp"
    "src/RotEnc.cpp"
    "src/System.cpp"
    "src/TextView.cpp"
    "src/ViewController.cpp"
    "src/netdisp.cpp"
  INCLUDE_DIRS
    "include"
  REQUIRES driver lcdgfx esp_wifi esp_event esp_netif nvs_flash console network esp32-rotary-encoder
)
