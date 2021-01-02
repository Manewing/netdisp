#include <netdisp/Display.hpp>

namespace netdisp {

void DisplayController::setFontStyle(FontStyle St) { Style = St; }

void DisplayController::toggleFontStyle(FontStyle St) {
  if (Style != St) {
    Style = St;
  } else {
    Style = FontStyle::NORMAL;
  }
}

} // namespace netdisp