#include <algorithm>
#include <cassert>
#include <netdisp/View.hpp>

namespace netdisp {

TextViewBase::TextViewBase(std::string Txt) : Text(std::move(Txt)) {}

RawTextView::RawTextView(std::string Txt) : TextViewBase(std::move(Txt)) {}

void RawTextView::show(DisplayController &DC) {
  DC.clear();
  DC.setFontStyle(DisplayController::FontStyle::NORMAL);
  DC.write(Text.c_str(), /*Line=*/0, /*Column=*/0, /*Wrap=*/true);
}

TextView::TextView(std::string Txt) : TextViewBase(std::move(Txt)) {}

void TextView::show(DisplayController &DC) {
  static const std::string FormatChars = "*_~\n";

  // Clear screen and set normal style
  DC.clear();
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

ViewController::ViewController(std::shared_ptr<View> DefView, unsigned MaxViews)
    : DefaultView(std::move(DefView)), Views() {
  Views.resize(MaxViews);
}

unsigned ViewController::getMaxViews() const { return Views.size(); }

void ViewController::show(DisplayController &DC) {
  // TODO add something like needsUpdate to view to avoid redraw
  if (!Views.at(ShownViewIdx)) {
    if (LastView != DefaultView.get()) {
      LastView = DefaultView.get();
      DefaultView->show(DC);
    }
    return;
  }

  if (LastView != Views.at(ShownViewIdx).get()) {
    LastView = Views.at(ShownViewIdx).get();
    Views.at(ShownViewIdx)->show(DC);
  }
}

bool ViewController::setView(unsigned Idx, std::shared_ptr<View> V) {
  if (Idx > getMaxViews()) {
    return false;
  }
  Views.at(Idx) = V;
  return true;
}

bool ViewController::selectCurrentViewIdx(unsigned Idx) {
  if (Idx > getMaxViews()) {
    return false;
  }
  CurrentViewIdx = Idx;
  return true;
}

unsigned ViewController::getCurrentViewIdx() const { return CurrentViewIdx; }

bool ViewController::showView(unsigned Idx) {
  if (Idx > getMaxViews()) {
    return false;
  }
  ShownViewIdx = Idx;
  return true;
}

unsigned ViewController::getShownViewIdx() const { return ShownViewIdx; }

} // namespace netdisp