#include <netdisp/Parser.hpp>

#include <cstring>

#include <esp_log.h>

namespace netdisp {

Parser::Parser(const uint8_t *Data, std::size_t Length)
    : ByteStream(Data, Length) {}

std::unique_ptr<Command> Parser::parse() {
  uint16_t Magic = 0;
  if (!get(Magic)) {
    return nullptr;
  }

  // FIXME byte order issue
  if (Magic != 0xfaef) {
    // No magic, treat data as text
    // TODO sanatize? max length?
    seek(0);
    std::string Text;
    get(Text, getLength());
    return std::unique_ptr<Command>(
        new ShowTextCmd(std::move(Text), false, false));
  }

  std::unique_ptr<Command> FirstCmd = parseNextCommand();
  if (!FirstCmd) {
    // TODO dump message content
    return nullptr;
  }
  Command *LastCmd = FirstCmd.get();

  while (!eof()) {
    std::unique_ptr<Command> NextCmd = parseNextCommand();
    if (!NextCmd) {
      // TODO dump message content
      return nullptr;
    }

    LastCmd = LastCmd->setNext(std::move(NextCmd));
  }

  return FirstCmd;
}

std::unique_ptr<Command> Parser::parseNextCommand() {
  uint8_t CmdId;
  if (!get(CmdId)) {
    return nullptr;
  }

  switch (CmdId) {
  case 0x00: {
    uint8_t Idx = 0;
    if (!get(Idx)) {
      return nullptr;
    }
    return std::unique_ptr<Command>(new SelectViewCmd(Idx));
  }
  case 0x01: {
    uint8_t Idx = 0;
    if (!get(Idx)) {
      return nullptr;
    }
    return std::unique_ptr<Command>(new ShowViewCmd(Idx));
  }
  case 0x02: {
    uint8_t LedInfo = 0;
    if (!get(LedInfo)) {
      return nullptr;
    }
    unsigned Led = (LedInfo >> 1) & 0xff;
    bool State = LedInfo & 0x1;
    return std::unique_ptr<Command>(new SetLedCmd(Led, State));
  }
  case 0x03: {
    uint8_t Raw = false, Length = 0;
    std::string Text;
    if (!get(Raw, Length) || !get(Text, Length)) {
      return nullptr;
    }
    return std::unique_ptr<Command>(new ShowTextCmd(std::move(Text), Raw));
  }
  case 0x04: {
    uint8_t Led = 0, Times = 0;
    if (!get(Led, Times)) {
      return nullptr;
    }
    return std::unique_ptr<Command>(new BlinkLedCmd(Led, Times));
  }
  case 0x5: {
    uint16_t X = 0, Y = 0, Width = 0, Height = 0, Length = 0;
    if (!get(X, Y, Width, Height, Length)) {
      return nullptr;
    }
    if (!hasData(Length)) {
      return nullptr;
    }
    auto Cmd = std::unique_ptr<Command>(
        new ShowBitmapCmd(X, Y, Width, Height, getData() + getPos(), Length));
    seek(getPos() + Length);
    return Cmd;
  }
  case 0x6: {
    return std::unique_ptr<Command>(new CreateCompositeViewCmd());
  }
  case 0x8: {
    return std::unique_ptr<Command>(new EndViewCmd());
  }

  default:
    break;
  }

  return nullptr;
}

} // namespace netdisp