#include <gtest/gtest.h>
#include <memory>
#include <netdisp/ByteStream.hpp>

namespace {

template <typename T> struct array_deleter {
  void operator()(T const *p) { delete[] p; }
};

template <typename Type> void pack_internal(uint8_t *Dest, const Type &T) {
  *reinterpret_cast<Type *>(Dest) = T;
}

template <typename Type, typename... ArgTypes>
void pack_internal(uint8_t *Dest, Type &T, const ArgTypes &... Args) {
  pack_internal(Dest, T);
  pack_internal(Dest + sizeof(T), Args...);
}

template <typename Type> std::size_t SizeOf(const Type &T) { return sizeof(T); }

template <typename Type, typename... ArgTypes>
std::size_t SizeOf(const Type &T, const ArgTypes &... Args) {
  return sizeof(T) + SizeOf(Args...);
}

template <typename Type, typename... ArgTypes>
std::pair<std::shared_ptr<uint8_t>, std::size_t>
pack(const Type &T, const ArgTypes &... Args) {
  std::size_t Length = SizeOf(T, Args...);
  std::shared_ptr<uint8_t> Data(new uint8_t[Length], array_deleter<uint8_t>());
  pack_internal(Data.get(), T, Args...);
  return {Data, Length};
}

} // namespace

TEST(TestByteStream, Atrributes) {
  const uint8_t *Text = reinterpret_cast<const uint8_t *>("Text text");
  netdisp::ByteStream BS(Text, sizeof(Text));

  EXPECT_EQ(BS.getData(), Text);
  EXPECT_TRUE(BS.hasData());
  EXPECT_TRUE(BS.hasData(sizeof(Text)));
  EXPECT_FALSE(BS.eof());
  EXPECT_EQ(BS.getPos(), 0);
}

TEST(TestByteStream, Integers) {
  uint8_t A = 0, ARef = 0xAA;
  uint16_t B = 0, BRef = 0xBBBB;
  uint32_t C = 0, CRef = 0xCCCCCCCC;
  uint64_t D = 0, DRef = 0xDDDDDDDDDDDDDDDD;
  int16_t E = 0, ERef = 0xEEEE;

  auto Data = pack(ARef, BRef, CRef, DRef, ERef);
  netdisp::ByteStream BS(Data.first.get(), Data.second);

  EXPECT_TRUE(BS.get(A, B));
  EXPECT_EQ(BS.getPos(), sizeof(A) + sizeof(B));
  EXPECT_TRUE(BS.get(C, D, E));
  EXPECT_EQ(A, ARef);
  EXPECT_EQ(B, BRef);
  EXPECT_EQ(C, CRef);
  EXPECT_EQ(D, DRef);
  EXPECT_EQ(E, ERef);
  EXPECT_TRUE(BS.eof());
}

TEST(TestByteStream, String) {
  std::string Text = "Text text";
  netdisp::ByteStream BS(reinterpret_cast<const uint8_t *>(Text.c_str()),
                         Text.size());

  std::string Str;
  EXPECT_TRUE(BS.get(Str, Text.size()));
  EXPECT_EQ(Str, Text);
  EXPECT_TRUE(BS.eof());
}