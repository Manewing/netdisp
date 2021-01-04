#ifndef NETDISP_VIEW_HPP
#define NETDISP_VIEW_HPP

#include <memory>
#include <vector>

namespace netdisp {
class DisplayController;
}

namespace netdisp {

class View {
public:
  void show(DisplayController &DC);

  bool isDirty() const;
  void dirty();

protected:
  virtual void showInternal(DisplayController &DC) = 0;

private:
  bool IsDirty = true;
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

protected:
  void showInternal(DisplayController &DC) override;
};

class TextView : public TextViewBase {
public:
  TextView(std::string Text);

protected:
  void showInternal(DisplayController &DC) override;
  void write(DisplayController &DC, std::size_t Start, std::size_t End);

private:
  unsigned Column = 0;
  unsigned Line = 0;
  bool WriteCentered = false;
};

class BitmapView : public View {
public:
  BitmapView(unsigned X, unsigned Y, unsigned Width, unsigned Height,
             const uint8_t *Data, unsigned Length);

protected:
  void showInternal(DisplayController &DC) override;

private:
  unsigned X;
  unsigned Y;
  unsigned Width;
  unsigned Height;
  std::unique_ptr<uint8_t> BitmapData;
};

// TODO refactor, take view to show as argument
class Notification : public View {
public:
  Notification(unsigned TimeoutMs);

  bool isTimedout() const;

private:
  unsigned TimeoutEndMs;
};

class IdxInfoView : public Notification {
public:
  IdxInfoView(unsigned Idx, unsigned TimeoutMs);

protected:
  void showInternal(DisplayController &DC) override;

private:
  unsigned Idx;
};

class ViewController {
public:
  // use uint8_t instead of unsigned
  ViewController(std::shared_ptr<View> DefaultView, unsigned MaxViews);

  unsigned getMaxViews() const;

  void show(DisplayController &DC);

  bool setView(unsigned Idx, std::shared_ptr<View> V);
  // delete view

  void setNotification(std::shared_ptr<Notification> N);
  void clearNotification();

  bool selectCurrentViewIdx(unsigned Idx);
  unsigned getCurrentViewIdx() const;

  bool showView(unsigned Idx);
  unsigned getShownViewIdx() const;

private:
  View &getViewToShow();

private:
  std::shared_ptr<View> DefaultView;
  std::shared_ptr<Notification> Notify;
  std::vector<std::shared_ptr<View>> Views;

  unsigned CurrentViewIdx = 0;
  unsigned ShownViewIdx = 0;
};

} // namespace netdisp

#endif // #ifndef NETDISP_VIEW_HPP