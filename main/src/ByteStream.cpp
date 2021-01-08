#include <algorithm>
#include <cstring>
#include <netdisp/ByteStream.hpp>

namespace netdisp {

ByteStream::ByteStream(const uint8_t *Data, std::size_t Length)
    : Data(Data), Length(Length), Pos(0) {}

bool ByteStream::hasData(std::size_t Count) const {
  return Length >= (Pos + Count);
}

bool ByteStream::get(std::string &Str, std::size_t Length) {
  if (!hasData(Length)) {
    return false;
  }
  Str.assign(reinterpret_cast<const char *>(Data) + Pos, Length);
  seek(getPos() + Length);
  return true;
}

bool ByteStream::eof() const { return Pos >= Length; }

std::size_t ByteStream::seek(std::size_t To) {
  Pos = std::min(Length, To);
  return Pos;
}

} // namespace netdisp