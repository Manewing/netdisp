#ifndef NETDISP_COMMAND_HPP
#define NETDISP_COMMAND_HPP

#include <memory>
#include <netdisp/CommandBase.hpp>

namespace netdisp {
class View;
class Context;
} // namespace netdisp

namespace netdisp {

class SelectViewCmd : public Command {
public:
  SelectViewCmd(unsigned Idx);

protected:
  void executeInternal(Context &Ctx) const override;

private:
  unsigned Idx;
};

class ShowViewCmd : public Command {
public:
  ShowViewCmd(unsigned Idx);

protected:
  void executeInternal(Context &Ctx) const override;

private:
  unsigned Idx;
};

class SetLedCmd : public Command {
public:
  SetLedCmd(unsigned Led, bool State);

protected:
  void executeInternal(Context &Ctx) const override;

private:
  unsigned Led;
  bool State;
};

class BlinkLedCmd : public Command {
public:
  BlinkLedCmd(unsigned Led, unsigned Times);

protected:
  void executeInternal(Context &Ctx) const override;

private:
  unsigned Led;
  unsigned Times;
};

class ViewCommand : public Command {
protected:
  void setView(Context &Ctx, unsigned Idx,
               const std::shared_ptr<View> &V) const;
};

class ShowTextCmd : public ViewCommand {
public:
  ShowTextCmd(std::string Text, bool Raw, bool UseCurrentView = true);

protected:
  void executeInternal(Context &Ctx) const override;

protected:
  bool UseCurrentView;
  std::shared_ptr<View> TxtView;
};

class ShowBitmapCmd : public ViewCommand {
public:
  ShowBitmapCmd(unsigned X, unsigned Y, unsigned Width, unsigned Height,
                const uint8_t *Data, unsigned Length);

protected:
  void executeInternal(Context &Ctx) const override;

private:
  std::shared_ptr<View> BmpView;
};

class CreateCompositeViewCmd : public ViewCommand {
public:
  CreateCompositeViewCmd();

protected:
  void executeInternal(Context &Ctx) const override;

protected:
  std::shared_ptr<View> CompView;
};

class CreateNotificationCmd : public ViewCommand {
public:
  CreateNotificationCmd(unsigned TimeoutMs);

protected:
  void executeInternal(Context &Ctx) const override;

protected:
  unsigned TimeoutMs;
  std::shared_ptr<View> Notification;
};

class EndViewCmd : public Command {
protected:
  void executeInternal(Context &Ctx) const override;
};

} // namespace netdisp

#endif // #ifndef NETDISP_COMMAND_HPP