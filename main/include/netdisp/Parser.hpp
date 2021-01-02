#ifndef NETDISP_PARSER_HPP
#define NETDISP_PARSER_HPP

#include <netdisp/Command.hpp>

namespace netdisp {

class ByteStream {
public:
  ByteStream(uint8_t *Data, std::size_t Length);
  virtual ~ByteStream() = default;

  const uint8_t *getData() const {
    return Data;
  }

  std::size_t getLength() const {
    return Length;
  }

  std::size_t getPos() const {
    return Pos;
  }

  std::size_t seek(std::size_t To);

  template <typename Type>
  bool get(Type &T) {
    return get(&T, sizeof(T));
  }

  bool eof() const;

protected:
  bool get(void *Dest, std::size_t Count);

protected:
  uint8_t *Data = nullptr;
  std::size_t Length = 0;
  std::size_t Pos = 0;
};

class Parser : protected ByteStream {
public:
  Parser(uint8_t *Data, std::size_t Length);

  std::unique_ptr<Command> parse();

private:
  std::unique_ptr<Command> parseNextCommand();

};

}

#endif // #ifndef NETDISP_PARSER_HPP