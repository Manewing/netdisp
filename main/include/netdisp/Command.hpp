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

class ShowTextCmd : public Command {
public:
  ShowTextCmd(std::string Text, bool Raw);

protected:
  void executeInternal(Context &Ctx) const override;

protected:
  std::shared_ptr<View> TxtView;
};

}

#endif // #ifndef NETDISP_COMMAND_HPP