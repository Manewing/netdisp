#include <netdisp/Parser.hpp>

#include <cstring>

#include <esp_log.h>

namespace netdisp {

ByteStream::ByteStream(uint8_t *Data, std::size_t Length)
    : Data(Data), Length(Length), Pos(0) {}

bool ByteStream::eof() const { return Pos >= Length; }

std::size_t ByteStream::seek(std::size_t To) {
  Pos = std::min(Length, To);
  return Pos;
}

bool ByteStream::get(void *Dest, std::size_t Count) {
  if (Length < (Pos + Count)) {
    return false;
  }
  std::memcpy(Dest, Data + Pos, Count);
  Pos += Count;
  return true;
}

Parser::Parser(uint8_t *Data, std::size_t Length) : ByteStream(Data, Length) {}

std::unique_ptr<Command> Parser::parse() {
  uint16_t Magic = 0;
  if (!get<uint16_t>(Magic)) {
    return nullptr;
  }

  // FIXME byte order issue
  if (Magic != 0xfaef) {
    // No magic, treat data as text
    // TODO sanatize? max length?
    seek(0);
    const char *Buffer = reinterpret_cast<const char *>(getData());
    std::string Text;
    Text.assign(Buffer, getLength());
    return std::unique_ptr<Command>(new ShowTextCmd(std::move(Text), false));
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
  //printf("pos: %d, len: %d, data: %s\n", getPos(), getLength(), getData());
  uint8_t CmdId;
  if (!get<uint8_t>(CmdId)) {
    return nullptr;
  }

  switch (CmdId) {
  case 0x00: {
    uint8_t Idx = 0;
    if (!get<uint8_t>(Idx)) {
      return nullptr;
    }
    return std::unique_ptr<Command>(new SelectViewCmd(Idx));
  }
  case 0x01: {
    uint8_t Idx = 0;
    if (!get<uint8_t>(Idx)) {
      return nullptr;
    }
    return std::unique_ptr<Command>(new ShowViewCmd(Idx));
  }
  case 0x02: {
    uint8_t LedInfo = 0;
    if (!get<uint8_t>(LedInfo)) {
      return nullptr;
    }
    unsigned Led = (LedInfo >> 1) & 0xff;
    bool State = LedInfo & 0x1;
    return std::unique_ptr<Command>(new SetLedCmd(Led, State));
  }
  case 0x03: {
    uint8_t Raw = false;
    if (!get<uint8_t>(Raw)) {
      return nullptr;
    }

    // Rest of data is text
    const char *Buffer = reinterpret_cast<const char *>(getData() + getPos());
    std::string Text;
    Text.assign(Buffer, getLength() - getPos());

    // take up rest of message buffer
    seek(getLength());

    return std::unique_ptr<Command>(new ShowTextCmd(std::move(Text), Raw));
  }
  default:
    break;
  }

  return nullptr;
}

} // namespace netdisp