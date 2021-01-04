#ifndef NETDISP_PARSER_HPP
#define NETDISP_PARSER_HPP

#include <netdisp/Command.hpp>
#include <type_traits>

namespace netdisp {

class ByteStream {
public:
  ByteStream(uint8_t *Data, std::size_t Length);
  virtual ~ByteStream() = default;

  const uint8_t *getData() const { return Data; }

  std::size_t getLength() const { return Length; }

  std::size_t getPos() const { return Pos; }

  bool hasData(std::size_t Count) const;

  std::size_t seek(std::size_t To);

  template <typename Type,
            typename std::enable_if<!std::is_trivially_copyable<Type>::value,
                                    bool>::type = true>
  bool get(Type &T) {
    return getData(&T, sizeof(T));
  }

  template <typename Type,
            typename std::enable_if<std::is_trivially_copyable<Type>::value,
                                    bool>::type = true>
  bool get(Type &T) {
    if (!hasData(sizeof(Type))) {
      return false;
    }
    T = *reinterpret_cast<Type *>(Data + Pos);
    Pos += sizeof(Type);
    return true;
  }

  template <typename Type, typename... ArgTypes>
  bool get(Type &T, ArgTypes &... Args) {
    return get(T) && get(Args...);
  }

  bool eof() const;

protected:
  bool getData(void *Dest, std::size_t Count);

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

} // namespace netdisp

#endif // #ifndef NETDISP_PARSER_HPP