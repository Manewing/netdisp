#ifndef UNITTEST_PACK_HPP
#define UNITTEST_PACK_HPP

#include <cstring>
#include <memory>
#include <string>

namespace unittest {

template <typename T> struct array_deleter {
  void operator()(T const *p) { delete[] p; }
};

template <typename Type> std::size_t SizeOf(const Type &T) { return sizeof(T); }

template <> inline std::size_t SizeOf<std::string>(const std::string &Str) {
  return sizeof(uint8_t) + Str.size();
}

template <typename Type, typename... ArgTypes>
std::size_t SizeOf(const Type &T, const ArgTypes &... Args) {
  return SizeOf(T) + SizeOf(Args...);
}

template <typename Type> void pack_elem(uint8_t *Dest, const Type &T) {
  *reinterpret_cast<Type *>(Dest) = T;
}

template <>
inline void pack_elem<std::string>(uint8_t *Dest, const std::string &Str) {
  Dest[0] = Str.size();
  std::memcpy(Dest + sizeof(uint8_t), Str.c_str(), Str.size());
}

template <typename Type, typename... ArgTypes>
void pack_elem(uint8_t *Dest, Type &T, const ArgTypes &... Args) {
  pack_elem(Dest, T);
  pack_elem(Dest + SizeOf(T), Args...);
}

template <typename Type, typename... ArgTypes>
std::pair<std::shared_ptr<uint8_t>, std::size_t>
pack(const Type &T, const ArgTypes &... Args) {
  std::size_t Length = SizeOf(T, Args...);
  std::shared_ptr<uint8_t> Data(new uint8_t[Length], array_deleter<uint8_t>());
  pack_elem(Data.get(), T, Args...);
  return {Data, Length};
}

template <typename Type> const uint8_t *data(const Type *T) {
  return reinterpret_cast<const uint8_t *>(T);
}

} // namespace unittest

#endif // #ifndef UNITTEST_PACK_HPP