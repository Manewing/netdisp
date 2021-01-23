#include <netdisp/Display.hpp>
#include <netdisp/Notification.hpp>

namespace netdisp {

IdxInfoView::IdxInfoView(unsigned Idx) : Idx(Idx) {}

void IdxInfoView::show(DisplayController &DC) {
  auto IdxStr = std::to_string(Idx);

  unsigned Line = DC.getLines() / 2;
  unsigned Column = (DC.getColumns() - IdxStr.size()) / 2;

  DC.setFontStyle(DisplayController::FontStyle::BOLD);
  DC.write(IdxStr.c_str(), Line, Column, /*Wrap=*/false);
}

} // namespace netdisp