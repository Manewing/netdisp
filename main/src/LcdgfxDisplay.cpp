#include <netdisp/LcdgfxDisplay.hpp>

namespace netdisp {

namespace {
EFontStyle getEFontStyle(DisplayController::FontStyle Style) {
  switch (Style) {
  case DisplayController::FontStyle::NORMAL:
    return STYLE_NORMAL;
  case DisplayController::FontStyle::BOLD:
    return STYLE_BOLD;
  case DisplayController::FontStyle::ITALIC:
    return STYLE_ITALIC;
  default:
    break;
  }
  return STYLE_NORMAL;
}
} // namespace

constexpr unsigned FontW = 8;
constexpr unsigned FontH = 16;

LcdgfxDisplayController::LcdgfxDisplayController() : Display(-1) {
  Display.begin();
  Display.setFixedFont(ssd1306xled_font8x16); // font WxH
  Display.fill(0xffff);
  Display.clear();
}

unsigned LcdgfxDisplayController::getColumns() const {
  // Broken interface Display::width should be const
  return const_cast<DisplaySSD1306_128x64_I2C *>(&Display)->width() / FontW;
}

unsigned LcdgfxDisplayController::getLines() const {
  // Broken interface Display::height should be const
  return const_cast<DisplaySSD1306_128x64_I2C *>(&Display)->height() / FontH;
}

void LcdgfxDisplayController::clear() { Display.clear(); }

void LcdgfxDisplayController::write(const std::string &Text, unsigned Line,
                                    unsigned Column, bool Wrap) {
  if (Column > getColumns() || Line > getLines() || Text.empty()) {
    return;
  }

  EFontStyle Efs = getEFontStyle(Style);

  if (Wrap) {
    Display.printFixed(Column * FontW, Line * FontH, Text.c_str(), Efs);
  } else {
    Display.printFixed(Column * FontW, Line * FontH,
                       Text.substr(0, getColumns()).c_str(), Efs);
  }
}

void LcdgfxDisplayController::drawBitmap(unsigned X, unsigned Y, unsigned Width,
                                         unsigned Height,
                                         const uint8_t *Bitmap) {
  Display.drawBitmap1(X, Y, Width, Height, Bitmap);
}

} // namespace netdisp