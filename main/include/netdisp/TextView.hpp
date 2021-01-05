#ifndef NETDISP_TEXT_VIEW_HPP
#define NETDISP_TEXT_VIEW_HPP

#include <netdisp/View.hpp>
#include <string>

namespace netdisp {
class DisplayController;
}

namespace netdisp {

class TextViewBase : public View {
public:
  TextViewBase(std::string Text);

protected:
  std::string Text;
};

class RawTextView : public TextViewBase {
public:
  RawTextView(std::string Text);

  void show(DisplayController &DC) override;
};

class TextView : public TextViewBase {
public:
  TextView(std::string Text);

  void show(DisplayController &DC) override;

protected:
  void write(DisplayController &DC, std::size_t Start, std::size_t End);

private:
  unsigned Column = 0;
  unsigned Line = 0;
  bool WriteCentered = false;
  bool WriteLarge = false;
};

} // namespace netdisp

#endif // #ifndef NETDISP_TEXT_VIEW_HPP