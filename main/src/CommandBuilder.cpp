#include <netdisp/CommandBuilder.hpp>

namespace netdisp {

std::shared_ptr<Command>
DefaultCommandBuilder::createSelectViewCmd(unsigned Idx) const {
  return std::shared_ptr<Command>(new SelectViewCmd(Idx));
}

std::shared_ptr<Command>
DefaultCommandBuilder::createShowViewCmd(unsigned Idx) const {
  return std::shared_ptr<Command>(new ShowViewCmd(Idx));
}

std::shared_ptr<Command>
DefaultCommandBuilder::createSetLedCmd(unsigned Led, bool State) const {
  return std::shared_ptr<Command>(new SetLedCmd(Led, State));
}

std::shared_ptr<Command>
DefaultCommandBuilder::createShowTextCmd(std::string Text, bool Raw,
                                         bool UseCurrentView) const {
  return std::shared_ptr<Command>(
      new ShowTextCmd(std::move(Text), Raw, UseCurrentView));
}

std::shared_ptr<Command>
DefaultCommandBuilder::createBlinkLedCmd(unsigned Led, unsigned Times) const {
  return std::shared_ptr<Command>(new BlinkLedCmd(Led, Times));
}

std::shared_ptr<Command> DefaultCommandBuilder::createShowBitmapCmd(
    unsigned X, unsigned Y, unsigned Width, unsigned Height,
    const uint8_t *Data, unsigned Length) const {
  return std::shared_ptr<Command>(
      new ShowBitmapCmd(X, Y, Width, Height, Data, Length));
}

std::shared_ptr<Command> DefaultCommandBuilder::createCompViewCmd() const {
  return std::shared_ptr<Command>(new CreateCompositeViewCmd());
}

std::shared_ptr<Command>
DefaultCommandBuilder::createNotificationCmd(unsigned TimeoutMs) const {
  return std::shared_ptr<Command>(new CreateNotificationCmd(TimeoutMs));
}

std::shared_ptr<Command> DefaultCommandBuilder::createEndViewCmd() const {
  return std::shared_ptr<Command>(new EndViewCmd());
}

} // namespace netdisp