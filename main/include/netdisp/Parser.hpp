#ifndef NETDISP_PARSER_HPP
#define NETDISP_PARSER_HPP

#include <netdisp/ByteStream.hpp>
#include <netdisp/Command.hpp>

namespace netdisp {
class Parser : protected ByteStream {
public:
  Parser(const uint8_t *Data, std::size_t Length);

  std::unique_ptr<Command> parse();

private:
  std::unique_ptr<Command> parseNextCommand();
};

} // namespace netdisp

#endif // #ifndef NETDISP_PARSER_HPP