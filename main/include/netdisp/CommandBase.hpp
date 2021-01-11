#ifndef NETDISP_COMMAND_BASE_HPP
#define NETDISP_COMMAND_BASE_HPP

#include <memory>

namespace netdisp {
class Context;
}

namespace netdisp {

class Command {
public:
  virtual ~Command() = default;

  Command *setNext(std::shared_ptr<Command> Next);

  void execute(Context &Ctx) const;

protected:
  virtual void executeInternal(Context &Ctx) const = 0;

private:
  std::shared_ptr<Command> NextCmd = nullptr;
};

} // namespace netdisp

#endif // #ifndef NETDISP_COMMAND_BASE_HPP