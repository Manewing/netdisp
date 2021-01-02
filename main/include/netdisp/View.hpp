#ifndef NETDISP_VIEW_HPP
#define NETDISP_VIEW_HPP

#include <memory>
#include <vector>

#include <netdisp/Display.hpp>

namespace netdisp {

class View {
public:
  virtual void show(DisplayController &DC) = 0;
};

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

private:
  void write(DisplayController &DC, std::size_t Start, std::size_t End);

private:
  unsigned Column = 0;
  unsigned Line = 0;
  bool WriteCentered = false;
};

class ViewController {
public:
  // use uint8_t instead of unsigned
  ViewController(std::shared_ptr<View> DefaultView, unsigned MaxViews);

  unsigned getMaxViews() const;

  void show(DisplayController &DC);

  bool setView(unsigned Idx, std::shared_ptr<View> V);
  // delete view

  bool selectCurrentViewIdx(unsigned Idx);
  unsigned getCurrentViewIdx() const;

  bool showView(unsigned Idx);
  unsigned getShownViewIdx() const;

private:
  View *LastView = nullptr;
  std::shared_ptr<View> DefaultView;
  std::vector<std::shared_ptr<View>> Views;

  unsigned CurrentViewIdx = 0;
  unsigned ShownViewIdx = 0;
};

} // namespace netdisp

#endif // #ifndef NETDISP_VIEW_HPP