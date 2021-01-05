#include <netdisp/LcdgfxDisplay.hpp>

#include <cstring>
#include <netdisp/ExtraFonts.h>
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

LcdgfxDisplayController::LcdgfxDisplayController()
    : Display(-1), CanvasData(new uint8_t[1024]),
      Canvas(128, 64, CanvasData.get()) {
  Display.begin();
  Display.clear();
  Canvas.clear();
  Canvas.setFixedFont(ssd1306xled_font8x16); // font WxH
  Canvas.setMode(CANVAS_MODE_TRANSPARENT);
}

unsigned LcdgfxDisplayController::getWidth() const { return 128; }

unsigned LcdgfxDisplayController::getHeight() const { return 64; }

unsigned LcdgfxDisplayController::getColumns() const {
  return getWidth() / FontW;
}

unsigned LcdgfxDisplayController::getLines() const {
  return getHeight() / FontH;
}

void LcdgfxDisplayController::clear() { Canvas.clear(); }

void LcdgfxDisplayController::write(const std::string &Text, unsigned Line,
                                    unsigned Column, bool Wrap, bool Large) {
  if (Column > getColumns() || Line > getLines() || Text.empty()) {
    return;
  }

  EFontStyle Efs = getEFontStyle(Style);

  unsigned OffsetH = 0;
  if (Large) {
    OffsetH = 5;
    Canvas.setFixedFont(ssd1306_font16x22);
  }

  if (Wrap) {
    Canvas.printFixed(Column * FontW, Line * FontH + OffsetH, Text.c_str(),
                      Efs);
  } else {
    Canvas.printFixed(Column * FontW, Line * FontH + OffsetH,
                      Text.substr(0, getColumns()).c_str(), Efs);
  }

  if (Large) {
    Canvas.setFixedFont(ssd1306xled_font8x16);
  }
}

void LcdgfxDisplayController::drawBitmap(unsigned X, unsigned Y, unsigned Width,
                                         unsigned Height,
                                         const uint8_t *Bitmap) {
  Canvas.drawBitmap1(X, Y, Width, Height, Bitmap);
}

void LcdgfxDisplayController::flush() { Display.drawCanvas(0, 0, Canvas); }

} // namespace netdisp