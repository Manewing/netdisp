#include <algorithm>
#include <cassert>
#include <cstring>
#include <netdisp/View.hpp>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <netdisp/Display.hpp>

namespace netdisp {

void View::show(DisplayController &DC, bool ClearDisp) {
  if (!isDirty()) {
    return;
  }
  if (ClearDisp) {
    DC.clear();
  }
  showInternal(DC);
  IsDirty = false;
}

bool View::isDirty() const { return IsDirty; }

void View::dirty() { IsDirty = true; }

TextViewBase::TextViewBase(std::string Txt) : Text(std::move(Txt)) {}

RawTextView::RawTextView(std::string Txt) : TextViewBase(std::move(Txt)) {}

void RawTextView::showInternal(DisplayController &DC) {
  DC.setFontStyle(DisplayController::FontStyle::NORMAL);
  DC.write(Text.c_str(), /*Line=*/0, /*Column=*/0, /*Wrap=*/true);
}

TextView::TextView(std::string Txt) : TextViewBase(std::move(Txt)) {}

void TextView::showInternal(DisplayController &DC) {
  static const std::string FormatChars = "*_~\n";

  // Clear screen and set normal style
  DC.setFontStyle(DisplayController::FontStyle::NORMAL);

  std::size_t Last = 0;
  Line = 0;
  Column = 0;
  WriteCentered = false;

  for (std::size_t Idx = 0; Idx < Text.size(); Idx++) {
    if (FormatChars.find(Text[Idx]) == std::string::npos) {
      continue;
    }

    // Actually output the text
    write(DC, Last, Idx);
    Last = Idx + 1;

    switch (Text[Idx]) {
    case '*':
      DC.toggleFontStyle(DisplayController::FontStyle::BOLD);
      break;
    case '_':
      DC.toggleFontStyle(DisplayController::FontStyle::ITALIC);
      break;
    case '\n':
      Column = 0;
      Line++;
      break;
    case '~':
      WriteCentered = true;
      break;
    default:
      break;
    }
  }

  write(DC, Last, Text.size());
}

void TextView::write(DisplayController &DC, std::size_t Start,
                     std::size_t End) {
  int Count = End - Start;
  if (Count <= 0) {
    return;
  }

  auto TextSection = Text.substr(Start, Count);
  if (WriteCentered) {
    Column = (DC.getColumns() - TextSection.size()) / 2;
  }
  DC.write(TextSection.c_str(), Line, Column, /*Wrap=*/false);

  Column += Count;
  WriteCentered = false;
}

BitmapView::BitmapView(unsigned X, unsigned Y, unsigned Width, unsigned Height,
                       const uint8_t *BmpData, unsigned Length)
    : X(X), Y(Y), Width(Width), Height(Height),
      BitmapData(new uint8_t[Length]) {
  std::memcpy(BitmapData.get(), BmpData, Length);
}

void BitmapView::showInternal(DisplayController &DC) {
  DC.drawBitmap(X, Y, Width, Height, BitmapData.get());
}

Notification::Notification(unsigned TimeoutMs)
    : TimeoutEndMs(xTaskGetTickCount() * portTICK_PERIOD_MS + TimeoutMs) {}

bool Notification::isTimedout() const {
  return TimeoutEndMs <= xTaskGetTickCount() * portTICK_PERIOD_MS;
}

IdxInfoView::IdxInfoView(unsigned Idx, unsigned TimeoutMs)
    : Notification(TimeoutMs), Idx(Idx) {}

void IdxInfoView::showInternal(DisplayController &DC) {
  auto IdxStr = std::to_string(Idx);

  unsigned Line = DC.getLines() / 2;
  unsigned Column = (DC.getColumns() - IdxStr.size()) / 2;

  DC.setFontStyle(DisplayController::FontStyle::BOLD);
  DC.write(IdxStr.c_str(), Line, Column, /*Wrap=*/false);
}

ViewController::ViewController(std::shared_ptr<View> DefView, unsigned MaxViews)
    : DefaultView(std::move(DefView)), Views() {
  Views.resize(MaxViews);
}

unsigned ViewController::getMaxViews() const { return Views.size(); }

void ViewController::show(DisplayController &DC) {
  if (Notify) {
    if (Notify->isTimedout()) {
      clearNotification();
    } else {
      Notify->show(DC);

      // Mark original view as dirty so its updated after notification is
      // cleared
      getViewToShow().dirty();
      return;
    }
  }

  // Get the view to show, its either the view at the current show index
  // or the default view
  auto &V = getViewToShow();
  V.show(DC);

  // If we do not show the default view, mark the default view as dirty so
  // its actually shown when switched to it
  if (&V != DefaultView.get()) {
    DefaultView->dirty();
  }
}

bool ViewController::setView(unsigned Idx, std::shared_ptr<View> V) {
  if (Idx >= getMaxViews()) {
    return false;
  }
  Views.at(Idx) = V;
  return true;
}

void ViewController::setNotification(std::shared_ptr<Notification> N) {
  Notify = N;
}

void ViewController::clearNotification() { Notify = nullptr; }

bool ViewController::selectCurrentViewIdx(unsigned Idx) {
  if (Idx >= getMaxViews()) {
    return false;
  }
  CurrentViewIdx = Idx;
  return true;
}

unsigned ViewController::getCurrentViewIdx() const { return CurrentViewIdx; }

bool ViewController::showView(unsigned Idx) {
  if (Idx >= getMaxViews()) {
    return false;
  }

  // Mark currently shown view as dirty so its actually shown when switched
  // back to it
  if (Views.at(ShownViewIdx)) {
    Views.at(ShownViewIdx)->dirty();
  }

  setNotification(std::make_shared<IdxInfoView>(Idx, /*TimeoutMs=*/500));
  ShownViewIdx = Idx;
  return true;
}

unsigned ViewController::getShownViewIdx() const { return ShownViewIdx; }

View &ViewController::getViewToShow() {
  if (!Views.at(ShownViewIdx)) {
    return *DefaultView;
  }
  return *Views.at(ShownViewIdx);
}

} // namespace netdisp