#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <netdisp/Display.hpp>
#include <netdisp/Notification.hpp>

namespace netdisp {

Notification::Notification(unsigned TimeoutMs)
    : TimeoutEndMs(xTaskGetTickCount() * portTICK_PERIOD_MS + TimeoutMs) {}

bool Notification::isTimedout() const {
  return TimeoutEndMs <= xTaskGetTickCount() * portTICK_PERIOD_MS;
}

IdxInfoView::IdxInfoView(unsigned Idx, unsigned TimeoutMs)
    : Notification(TimeoutMs), Idx(Idx) {}

void IdxInfoView::show(DisplayController &DC) {
  auto IdxStr = std::to_string(Idx);

  unsigned Line = DC.getLines() / 2;
  unsigned Column = (DC.getColumns() - IdxStr.size()) / 2;

  DC.setFontStyle(DisplayController::FontStyle::BOLD);
  DC.write(IdxStr.c_str(), Line, Column, /*Wrap=*/false);
}

} // namespace netdisp