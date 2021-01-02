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

  virtual unsigned getLines() const = 0;
  virtual unsigned getColumns() const = 0;

  virtual void clear() = 0;
  virtual void write(const std::string &Text, unsigned Line = 0,
                     unsigned Column = 0, bool Wrap = false) = 0;

protected:
  FontStyle Style;
};

} // namespace netdisp

#endif // #ifndef NETDISP_DISPLAY_HPP