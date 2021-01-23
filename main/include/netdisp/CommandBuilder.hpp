#ifndef NETDISP_COMMAND_BUILDER_HPP
#define NETDISP_COMMAND_BUILDER_HPP

#include <netdisp/Command.hpp>

namespace netdisp {

class CommandBuilder {
public:
  virtual std::shared_ptr<Command> createSelectViewCmd(unsigned Idx) const = 0;
  virtual std::shared_ptr<Command> createShowViewCmd(unsigned Idx) const = 0;
  virtual std::shared_ptr<Command> createSetLedCmd(unsigned Led,
                                                   bool State) const = 0;
  virtual std::shared_ptr<Command> createBlinkLedCmd(unsigned Led,
                                                     unsigned Times) const = 0;
  virtual std::shared_ptr<Command>
  createShowTextCmd(std::string Text, bool Raw,
                    bool UseCurrentView = true) const = 0;
  virtual std::shared_ptr<Command>
  createShowBitmapCmd(unsigned X, unsigned Y, unsigned Width, unsigned Height,
                      const uint8_t *Data, unsigned Length) const = 0;
  virtual std::shared_ptr<Command> createCompViewCmd() const = 0;
  virtual std::shared_ptr<Command>
  createNotificationCmd(unsigned TimeoutMs) const = 0;
  virtual std::shared_ptr<Command> createEndViewCmd() const = 0;
};

class DefaultCommandBuilder : public CommandBuilder {
public:
  std::shared_ptr<Command> createSelectViewCmd(unsigned Idx) const override;
  std::shared_ptr<Command> createShowViewCmd(unsigned Idx) const override;
  std::shared_ptr<Command> createSetLedCmd(unsigned Led,
                                           bool State) const override;
  std::shared_ptr<Command> createBlinkLedCmd(unsigned Led,
                                             unsigned Times) const override;
  std::shared_ptr<Command>
  createShowTextCmd(std::string Text, bool Raw,
                    bool UseCurrentView = true) const override;
  std::shared_ptr<Command> createShowBitmapCmd(unsigned X, unsigned Y,
                                               unsigned Width, unsigned Height,
                                               const uint8_t *Data,
                                               unsigned Length) const override;
  std::shared_ptr<Command> createCompViewCmd() const override;
  std::shared_ptr<Command>
  createNotificationCmd(unsigned TimeoutMs) const override;
  std::shared_ptr<Command> createEndViewCmd() const override;
};

} // namespace netdisp

#endif // #ifndef NETDISP_COMMAND_BUILDER_HPP