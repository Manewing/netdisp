#ifndef NETDISP_BYTE_STREAM_HPP
#define NETDISP_BYTE_STREAM_HPP

#include <stdint.h>
#include <string>
#include <type_traits>

namespace netdisp {

/// Class that wraps around a read-only byte array and allows to stream data
/// from it.
class ByteStream {
public:
  ByteStream(const uint8_t *Data, std::size_t Length);

  virtual ~ByteStream() = default;

  const uint8_t *getData() const { return Data; }

  std::size_t getLength() const { return Length; }

  std::size_t getPos() const { return Pos; }

  bool hasData(std::size_t Count = 0) const;

  /// Sets position of stream and returns the selected position
  std::size_t seek(std::size_t To);

  /// Get a trivially copy-able type from the stream, e.g get<uint16_t>(Var)
  /// \return false if there is not enough data, else true
  template <typename Type,
            typename std::enable_if<std::is_trivially_copyable<Type>::value,
                                    bool>::type = true>
  bool get(Type &T) {
    if (!hasData(sizeof(Type))) {
      return false;
    }
    T = *reinterpret_cast<const Type *>(Data + Pos);
    Pos += sizeof(Type);
    return true;
  }

  /// Get multiple trivally copy-able types from stream
  /// \return false if there is not enough data, else true
  template <typename Type, typename... ArgTypes,
            typename std::enable_if<std::is_trivially_copyable<Type>::value,
                                    bool>::type = true>
  bool get(Type &T, ArgTypes &... Args) {
    return get(T) && get(Args...);
  }

  bool get(std::string &Str, std::size_t Length);

  bool eof() const;

protected:
  const uint8_t *Data = nullptr;
  std::size_t Length = 0;
  std::size_t Pos = 0;
};

} // namespace netdisp

#endif // #ifndef NETDISP_BYTE_STREAM_HPP