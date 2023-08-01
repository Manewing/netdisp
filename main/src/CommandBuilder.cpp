#include <cassert>
#include <netdisp/CommandBuilder.hpp>
#include <string>

namespace netdisp {

std::shared_ptr<Command>
DefaultCommandBuilder::createSelectViewCmd(unsigned Idx) const {
  auto Ret = std::shared_ptr<Command>(new SelectViewCmd(Idx));
  assert(Ret);
  return Ret;
}

std::shared_ptr<Command>
DefaultCommandBuilder::createShowViewCmd(unsigned Idx) const {
  auto Ret = std::shared_ptr<Command>(new ShowViewCmd(Idx));
  assert(Ret);
  return Ret;
}

std::shared_ptr<Command>
DefaultCommandBuilder::createSetLedCmd(unsigned Led, bool State) const {
  auto Ret = std::shared_ptr<Command>(new SetLedCmd(Led, State));
  assert(Ret);
  return Ret;
}

std::shared_ptr<Command>
DefaultCommandBuilder::createShowTextCmd(std::string Text, bool Raw,
                                         bool UseCurrentView) const {
  auto Ret = std::shared_ptr<Command>(
      new ShowTextCmd(std::move(Text), Raw, UseCurrentView));
  assert(Ret);
  return Ret;
}

std::shared_ptr<Command>
DefaultCommandBuilder::createBlinkLedCmd(unsigned Led, unsigned Times) const {
  auto Ret = std::shared_ptr<Command>(new BlinkLedCmd(Led, Times));
  assert(Ret);
  return Ret;
}

std::shared_ptr<Command> DefaultCommandBuilder::createShowBitmapCmd(
    unsigned X, unsigned Y, unsigned Width, unsigned Height,
    const uint8_t *Data, unsigned Length) const {
  auto Ret = std::shared_ptr<Command>(
      new ShowBitmapCmd(X, Y, Width, Height, Data, Length));
  assert(Ret);
  return Ret;
}

std::shared_ptr<Command>
DefaultCommandBuilder::createDrawLineCmd(int X0, int Y0, int X1, int Y1) const {
  auto Ret = std::shared_ptr<Command>(new DrawLineCmd(X0, Y0, X1, Y1));
  assert(Ret);
  return Ret;
}

std::shared_ptr<Command>
DefaultCommandBuilder::createDrawRectCmd(int X, int Y, unsigned W,
                                         unsigned H) const {
  auto Ret = std::shared_ptr<Command>(new DrawRectCmd(X, Y, W, H));
  assert(Ret);
  return Ret;
}

std::shared_ptr<Command>
DefaultCommandBuilder::createDrawCircleCmd(int X, int Y,
                                           unsigned Radius) const {
  auto Ret = std::shared_ptr<Command>(new DrawCircleCmd(X, Y, Radius));
  assert(Ret);
  return Ret;
}

std::shared_ptr<Command> DefaultCommandBuilder::createCompViewCmd() const {
  auto Ret = std::shared_ptr<Command>(new CreateCompositeViewCmd());
  assert(Ret);
  return Ret;
}

std::shared_ptr<Command>
DefaultCommandBuilder::createNotificationCmd(unsigned TimeoutMs) const {
  auto Ret = std::shared_ptr<Command>(new CreateNotificationCmd(TimeoutMs));
  assert(Ret);
  return Ret;
}

std::shared_ptr<Command> DefaultCommandBuilder::createEndViewCmd() const {
  auto Ret = std::shared_ptr<Command>(new EndViewCmd());
  assert(Ret);
  return Ret;
}

} // namespace netdisp