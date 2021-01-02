#ifndef NETDISP_LCDGFX_DISPLAY_HPP
#define NETDISP_LCDGFX_DISPLAY_HPP

#include <netdisp/Display.hpp>

#include <lcdgfx.h>

namespace netdisp {

class LcdgfxDisplayController : public DisplayController {
public:
  LcdgfxDisplayController();

  unsigned getLines() const override;
  unsigned getColumns() const override;

  void clear() override;
  void write(const std::string &Text, unsigned Line = 0, unsigned Column = 0,
             bool Wrap = false) override;

private:
  DisplaySSD1306_128x64_I2C Display;
};

} // namespace netdisp

#endif // #ifndef NETDISP_LCDGFX_DISPLAY_HPP