#include <algorithm>
#include <netdisp/Parser.hpp>

#include <cstring>

namespace netdisp {

const uint16_t Parser::Magic = 0xfaef;

Parser::Parser(const uint8_t *Data, std::size_t Length,
               const CommandBuilder &CB)
    : ByteStream(Data, Length), CB(CB) {}

std::shared_ptr<Command> Parser::parse() {
  uint16_t MagicVal = 0;
  if (!get(MagicVal)) {
    return nullptr;
  }

  // If we have no magic, treat data as text
  if (MagicVal != Magic) {
    // TODO sanatize? max length?
    seek(0);
    std::string Text;
    get(Text, getLength());
    std::replace_if(Text.begin(), Text.end(),
                    [](char C) { return !std::isprint(C) && C != '\n'; }, ' ');
    return CB.createShowTextCmd(std::move(Text), false, false);
  }

  std::shared_ptr<Command> FirstCmd = parseNextCommand();
  if (!FirstCmd) {
    // TODO dump message content
    return nullptr;
  }
  Command *LastCmd = FirstCmd.get();

  while (!eof()) {
    std::shared_ptr<Command> NextCmd = parseNextCommand();
    if (!NextCmd) {
      // TODO dump message content
      return nullptr;
    }

    LastCmd = LastCmd->setNext(NextCmd);
  }

  return FirstCmd;
}

std::shared_ptr<Command> Parser::parseNextCommand() {
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
    return CB.createSelectViewCmd(Idx);
  }
  case 0x01: {
    uint8_t Idx = 0;
    if (!get(Idx)) {
      return nullptr;
    }
    return CB.createShowViewCmd(Idx);
  }
  case 0x02: {
    uint8_t LedInfo = 0;
    if (!get(LedInfo)) {
      return nullptr;
    }
    unsigned Led = (LedInfo >> 1) & 0xff;
    bool State = LedInfo & 0x1;
    return CB.createSetLedCmd(Led, State);
  }
  case 0x03: {
    uint8_t Raw = false, Length = 0;
    std::string Text;
    if (!get(Raw, Length) || !get(Text, Length)) {
      return nullptr;
    }
    return CB.createShowTextCmd(std::move(Text), Raw);
  }
  case 0x04: {
    uint8_t Led = 0, Times = 0;
    if (!get(Led, Times)) {
      return nullptr;
    }
    return CB.createBlinkLedCmd(Led, Times);
  }
  case 0x5: {
    uint16_t X = 0, Y = 0, Width = 0, Height = 0, Length = 0;
    if (!get(X, Y, Width, Height, Length)) {
      return nullptr;
    }
    if (!hasData(Length)) {
      return nullptr;
    }
    auto Cmd = CB.createShowBitmapCmd(X, Y, Width, Height, getData() + getPos(),
                                      Length);
    seek(getPos() + Length);
    return Cmd;
  }
  case 0x6: {
    return CB.createCompViewCmd();
  }
  case 0x7: {
    uint16_t TimeoutMs = 0;
    if (!get(TimeoutMs)) {
      return nullptr;
    }
    return CB.createNotificationCmd(TimeoutMs);
  }
  case 0x8: {
    return CB.createEndViewCmd();
  }

  default:
    break;
  }

  return nullptr;
}

} // namespace netdisp