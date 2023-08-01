#ifndef NETDISP_PARSER_HPP
#define NETDISP_PARSER_HPP

#include <netdisp/ByteStream.hpp>
#include <netdisp/Command.hpp>
#include <netdisp/CommandBuilder.hpp>
#include <vector>

namespace netdisp {
class Parser : protected ByteStream {
public:
  static const uint16_t Magic;

public:
  Parser(const uint8_t *Data, std::size_t Length, const CommandBuilder &CB);

  std::vector<std::shared_ptr<Command>> parse();

private:
  std::shared_ptr<Command> parseNextCommand();

private:
  const CommandBuilder &CB;
};

} // namespace netdisp

#endif // #ifndef NETDISP_PARSER_HPP