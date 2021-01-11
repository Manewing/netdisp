#include <netdisp/CommandBase.hpp>

namespace netdisp {

Command *Command::setNext(std::shared_ptr<Command> Next) {
  NextCmd = std::move(Next);
  return NextCmd.get();
}

void Command::execute(Context &Ctx) const {
  executeInternal(Ctx);
  if (NextCmd) {
    NextCmd->execute(Ctx);
  }
}

} // namespace netdisp