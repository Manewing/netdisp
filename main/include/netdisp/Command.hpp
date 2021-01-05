#ifndef NETDISP_COMMAND_HPP
#define NETDISP_COMMAND_HPP

#include <memory>
#include <netdisp/Context.hpp>

namespace netdisp {
class View;
}

namespace netdisp {

class Command {
public:
  virtual ~Command() = default;

  Command *setNext(std::unique_ptr<Command> Next);

  void execute(Context &Ctx) const;

protected:
  virtual void executeInternal(Context &Ctx) const = 0;

private:
  std::unique_ptr<Command> NextCmd = nullptr;
};

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

class ShowTextCmd : public Command {
public:
  ShowTextCmd(std::string Text, bool Raw, bool UseCurrentView = true);

protected:
  void executeInternal(Context &Ctx) const override;

protected:
  bool UseCurrentView;
  std::shared_ptr<View> TxtView;
};

class ShowBitmapCmd : public Command {
public:
  ShowBitmapCmd(unsigned X, unsigned Y, unsigned Width, unsigned Height,
                const uint8_t *Data, unsigned Length);

protected:
  void executeInternal(Context &Ctx) const override;

private:
  std::shared_ptr<View> BmpView;
};

} // namespace netdisp

#endif // #ifndef NETDISP_COMMAND_HPP