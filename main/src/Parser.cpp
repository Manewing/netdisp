#include <algorithm>
#include <netdisp/Parser.hpp>

#include <cstring>

namespace netdisp {

const uint16_t Parser::Magic = 0xfaef;

Parser::Parser(const uint8_t *Data, std::size_t Length,
               const CommandBuilder &CB)
    : ByteStream(Data, Length), CB(CB) {}

std::vector<std::shared_ptr<Command>> Parser::parse() {
  uint16_t MagicVal = 0;
  if (!get(MagicVal)) {
    return {};
  }

  // If we have no magic, treat data as text
  if (MagicVal != Magic) {
    // TODO sanatize? max length?
    seek(0);
    std::string Text;
    get(Text, getLength());
    std::replace_if(Text.begin(), Text.end(),
                    [](char C) { return !std::isprint(C) && C != '\n'; }, ' ');
    return {CB.createShowTextCmd(std::move(Text), false, false)};
  }

  std::vector<std::shared_ptr<Command>> Commands;
  std::shared_ptr<Command> FirstCmd = parseNextCommand();
  if (!FirstCmd) {
    // TODO dump message content
    return {};
  }
  Commands.push_back(FirstCmd);

  while (!eof()) {
    std::shared_ptr<Command> NextCmd = parseNextCommand();
    if (!NextCmd) {
      // Parsing of package failed, abort
      return {};
    }

    Commands.push_back(NextCmd);
  }

  return Commands;
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
  case 0x05: {
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
  case 0x06: {
    return CB.createCompViewCmd();
  }
  case 0x07: {
    uint16_t TimeoutMs = 0;
    if (!get(TimeoutMs)) {
      return nullptr;
    }
    return CB.createNotificationCmd(TimeoutMs);
  }
  case 0x08: {
    return CB.createEndViewCmd();
  }
  case 0x09: {
    int16_t X0 = 0, Y0 = 0, X1 = 0, Y1 = 0;
    if (!get(X0, Y0, X1, Y1)) {
      return nullptr;
    }
    return CB.createDrawLineCmd(X0, Y0, X1, Y1);
  }
  case 0x0a: {
    int16_t X = 0, Y = 0;
    uint16_t W = 0, H = 0;
    if (!get(X, Y, W, H)) {
      return nullptr;
    }
    return CB.createDrawRectCmd(X, Y, W, H);
  }
  case 0x0b: {
    int16_t X = 0, Y = 0;
    uint16_t Radius = 0;
    if (!get(X, Y, Radius)) {
      return nullptr;
    }
    return CB.createDrawCircleCmd(X, Y, Radius);
  }

  default:
    break;
  }

  return nullptr;
}

} // namespace netdisp