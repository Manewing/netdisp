#ifndef NETDISP_DISPLAY_HPP
#define NETDISP_DISPLAY_HPP

#include <string>

namespace netdisp {

class DisplayController {
public:
  enum FontStyle { NORMAL, BOLD, ITALIC };

public:
  void setFontStyle(FontStyle Style);
  void toggleFontStyle(FontStyle Style);

  virtual unsigned getWidth() const = 0;
  virtual unsigned getHeight() const = 0;
  virtual unsigned getLines() const = 0;
  virtual unsigned getColumns() const = 0;

  virtual void clear() = 0;
  virtual void write(const std::string &Text, unsigned Line = 0,
                     unsigned Column = 0, bool Wrap = false,
                     bool Large = false) = 0;
  virtual void drawBitmap(unsigned X, unsigned Y, unsigned Width,
                          unsigned Height, const uint8_t *Bitmap) = 0;
  virtual void flush() = 0;

protected:
  FontStyle Style;
};

} // namespace netdisp

#endif // #ifndef NETDISP_DISPLAY_HPP