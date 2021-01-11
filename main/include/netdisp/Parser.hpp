#ifndef NETDISP_PARSER_HPP
#define NETDISP_PARSER_HPP

#include <netdisp/ByteStream.hpp>
#include <netdisp/Command.hpp>
#include <netdisp/CommandBuilder.hpp>

namespace netdisp {
class Parser : protected ByteStream {
public:
  Parser(const uint8_t *Data, std::size_t Length,
         const CommandBuilder &CB);

  std::unique_ptr<Command> parse();

private:
  std::unique_ptr<Command> parseNextCommand();

private:
  const CommandBuilder &CB;
};

} // namespace netdisp

#endif // #ifndef NETDISP_PARSER_HPP