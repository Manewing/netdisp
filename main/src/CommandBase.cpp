#include <netdisp/CommandBase.hpp>

#include <netdisp/Log.hpp>
namespace netdisp {

Command::~Command() {
  ND_LOGI("Command", "deleting %p", this);
}

void Command::execute(Context &Ctx) const {
  executeInternal(Ctx);
}

} // namespace netdisp