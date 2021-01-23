#ifndef NETDISP_LCDGFX_DISPLAY_HPP
#define NETDISP_LCDGFX_DISPLAY_HPP

#include <memory>
#include <netdisp/Display.hpp>

#include <lcdgfx.h>

namespace netdisp {

class LcdgfxDisplayController : public DisplayController {
public:
  LcdgfxDisplayController();

  unsigned getWidth() const override;
  unsigned getHeight() const override;
  unsigned getLines() const override;
  unsigned getColumns() const override;

  void clear() override;
  void write(const std::string &Text, unsigned Line = 0, unsigned Column = 0,
             bool Wrap = false, bool Large = false) override;
  void drawBitmap(unsigned X, unsigned Y, unsigned Width, unsigned Height,
                  const uint8_t *Bitmap) override;
  void drawLine(int X0, int Y0, int X1, int Y1) override;
  void drawRect(int X, int Y, unsigned W, unsigned H) override;
  void drawCircle(int X, int Y, unsigned Radius) override;
  void flush() override;

private:
  DisplaySSD1306_128x64_I2C Display;
  std::unique_ptr<uint8_t[]> CanvasData;
  NanoCanvas1 Canvas;
};

} // namespace netdisp

#endif // #ifndef NETDISP_LCDGFX_DISPLAY_HPP