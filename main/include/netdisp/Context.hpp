#ifndef NETDISP_CONTEXT_HPP
#define NETDISP_CONTEXT_HPP

#include <mutex>

namespace netdisp {
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

  void lock() { Mtx.lock(); }

  void unlock() { Mtx.unlock(); }

private:
  std::mutex Mtx;
};

} // namespace netdisp

#endif // #ifndef NETDISP_CONTEXT_HPP