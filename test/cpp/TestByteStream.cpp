#include <gtest/gtest.h>
#include <netdisp/ByteStream.hpp>
#include "Pack.hpp"


TEST(TestByteStream, Atrributes) {
  const uint8_t *Text = unittest::data("Text text");
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

  auto Data = unittest::pack(ARef, BRef, CRef, DRef, ERef);
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
  netdisp::ByteStream BS(unittest::data(Text.c_str()),
                         Text.size());

  std::string Str;
  EXPECT_TRUE(BS.get(Str, Text.size()));
  EXPECT_EQ(Str, Text);
  EXPECT_TRUE(BS.eof());

  EXPECT_FALSE(BS.get(Str, Text.size()));

  EXPECT_EQ(BS.seek(0), 0);
  EXPECT_TRUE(BS.get(Str, Text.size()));
  EXPECT_EQ(Str, Text);
  EXPECT_TRUE(BS.eof());
}