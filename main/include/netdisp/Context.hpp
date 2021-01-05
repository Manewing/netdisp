#ifndef NETDISP_CONTEXT_HPP
#define NETDISP_CONTEXT_HPP

#include <mutex>
#include <stack>

namespace netdisp {
class View;
class LedController;
class ViewController;
} // namespace netdisp

namespace netdisp {

class Context {
public:
  Context(LedController &LCtrl, ViewController &VCtrl)
      : LedCtrl(LCtrl), ViewCtrl(VCtrl) {}

  LedController &LedCtrl;
  ViewController &ViewCtrl;
  std::stack<std::shared_ptr<View>> ParentViews;

  void lock() { Mtx.lock(); }

  void unlock() {
    ParentViews = {};
    Mtx.unlock();
  }

private:
  std::mutex Mtx;
};

} // namespace netdisp

#endif // #ifndef NETDISP_CONTEXT_HPP