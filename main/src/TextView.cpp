#include <netdisp/Display.hpp>
#include <netdisp/TextView.hpp>

namespace netdisp {

TextViewBase::TextViewBase(std::string Txt) : Text(std::move(Txt)) {}

RawTextView::RawTextView(std::string Txt) : TextViewBase(std::move(Txt)) {}

void RawTextView::show(DisplayController &DC) {
  DC.setFontStyle(DisplayController::FontStyle::NORMAL);
  DC.write(Text.c_str(), /*Line=*/0, /*Column=*/0, /*Wrap=*/true);
}

TextView::TextView(std::string Txt) : TextViewBase(std::move(Txt)) {}

void TextView::show(DisplayController &DC) {
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

} // namespace netdisp