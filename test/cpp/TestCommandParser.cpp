#include "Pack.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <netdisp/Parser.hpp>

using testing::Return;

namespace netdisp {
class Context {};
} // namespace netdisp

class DummyCommand : public netdisp::Command {
protected:
  void executeInternal(netdisp::Context &) const override {}
};

std::shared_ptr<netdisp::Command> getDummyCmd() {
  return std::shared_ptr<netdisp::Command>(new DummyCommand());
}

class MockCmdBuilder : public netdisp::CommandBuilder {
public:
  MOCK_METHOD(std::shared_ptr<netdisp::Command>, createSelectViewCmd,
              (unsigned Idx), (const override));
  MOCK_METHOD(std::shared_ptr<netdisp::Command>, createShowViewCmd,
              (unsigned Idx), (const override));
  MOCK_METHOD(std::shared_ptr<netdisp::Command>, createSetLedCmd,
              (unsigned Led, bool State), (const override));
  MOCK_METHOD(std::shared_ptr<netdisp::Command>, createBlinkLedCmd,
              (unsigned Led, unsigned Times), (const override));
  MOCK_METHOD(std::shared_ptr<netdisp::Command>, createShowTextCmd,
              (std::string Text, bool Raw, bool UseCurrentView),
              (const override));
  MOCK_METHOD(std::shared_ptr<netdisp::Command>, createShowBitmapCmd,
              (unsigned X, unsigned Y, unsigned Width, unsigned Height,
               const uint8_t *Data, unsigned Length),
              (const override));
  MOCK_METHOD(std::shared_ptr<netdisp::Command>, createDrawLineCmd,
              (int X0, int Y0, int X1, int Y1), (const override));
  MOCK_METHOD(std::shared_ptr<netdisp::Command>, createDrawRectCmd,
              (int X, int Y, unsigned W, unsigned H), (const override));
  MOCK_METHOD(std::shared_ptr<netdisp::Command>, createDrawCircleCmd,
              (int X, int Y, unsigned Radius), (const override));
  MOCK_METHOD(std::shared_ptr<netdisp::Command>, createCompViewCmd, (),
              (const override));
  MOCK_METHOD(std::shared_ptr<netdisp::Command>, createNotificationCmd,
              (unsigned TimeoutMs), (const override));
  MOCK_METHOD(std::shared_ptr<netdisp::Command>, createEndViewCmd, (),
              (const override));
};

TEST(TestCommandParser, RawText) {
  {
    std::string Text = "Hello World";

    MockCmdBuilder MCB;
    EXPECT_CALL(MCB, createShowTextCmd(Text, false, false)).Times(1);

    netdisp::Parser P(unittest::data(Text.c_str()), Text.size(), MCB);
    P.parse();
  }
  {
    uint8_t Text[] = {0x00, 0x01, 0x41, 0x42, 0xf5, 0x13};

    MockCmdBuilder MCB;
    EXPECT_CALL(MCB, createShowTextCmd("  AB  ", false, false)).Times(1);

    netdisp::Parser P(unittest::data(Text), sizeof(Text), MCB);
    P.parse();
  }
}

TEST(TestCommandParser, ViewsAndLeds) {
  {
    MockCmdBuilder MCB;
    EXPECT_CALL(MCB, createSelectViewCmd(0x13)).Times(1);

    uint8_t Cmd[] = {0x00, 0x00, 0x00, 0x13};
    unittest::pack_elem(Cmd, netdisp::Parser::Magic);
    netdisp::Parser P(unittest::data(Cmd), sizeof(Cmd), MCB);
    P.parse();
  }
  {
    MockCmdBuilder MCB;
    EXPECT_CALL(MCB, createShowViewCmd(0x13)).Times(1);

    uint8_t Cmd[] = {0x00, 0x00, 0x01, 0x13};
    unittest::pack_elem(Cmd, netdisp::Parser::Magic);
    netdisp::Parser P(unittest::data(Cmd), sizeof(Cmd), MCB);
    P.parse();
  }
  {
    MockCmdBuilder MCB;
    EXPECT_CALL(MCB, createSetLedCmd(0x01, true)).Times(1);

    uint8_t Cmd[] = {0x00, 0x00, 0x02, 0x03};
    unittest::pack_elem(Cmd, netdisp::Parser::Magic);
    netdisp::Parser P(unittest::data(Cmd), sizeof(Cmd), MCB);
    P.parse();
  }
  {
    MockCmdBuilder MCB;
    EXPECT_CALL(MCB, createBlinkLedCmd(0x03, 0x04)).Times(1);

    uint8_t Cmd[] = {0x00, 0x00, 0x04, 0x03, 0x04};
    unittest::pack_elem(Cmd, netdisp::Parser::Magic);
    netdisp::Parser P(unittest::data(Cmd), sizeof(Cmd), MCB);
    P.parse();
  }
  {
    MockCmdBuilder MCB;
    EXPECT_CALL(MCB, createCompViewCmd()).WillOnce(Return(getDummyCmd()));
    EXPECT_CALL(MCB, createNotificationCmd(500))
        .WillOnce(Return(getDummyCmd()));
    EXPECT_CALL(MCB, createEndViewCmd()).WillOnce(Return(getDummyCmd()));

    uint8_t CompViewCmd = 0x06;
    uint8_t NotifyCmd = 0x07;
    uint8_t EndViewCmd = 0x08;
    uint16_t TimeoutMs = 500;
    auto Data = unittest::pack(netdisp::Parser::Magic, CompViewCmd, NotifyCmd,
                               TimeoutMs, EndViewCmd);
    netdisp::Parser P(Data.first.get(), Data.second, MCB);
    P.parse();
  }
}

TEST(TestCommandParser, ShowText) {
  MockCmdBuilder MCB;
  std::string Test = "Test test";
  std::string Abc = "Abc def";
  EXPECT_CALL(MCB, createShowTextCmd(Test, false, true))
      .WillOnce(Return(getDummyCmd()));
  EXPECT_CALL(MCB, createShowTextCmd(Abc, true, true))
      .WillOnce(Return(getDummyCmd()));

  uint8_t NotRaw = 0x00;
  uint8_t ShowTextCmd = 0x03;
  uint8_t Raw = 0x01;
  auto Data = unittest::pack(netdisp::Parser::Magic, ShowTextCmd, NotRaw, Test,
                             ShowTextCmd, Raw, Abc);
  netdisp::Parser P(Data.first.get(), Data.second, MCB);
  P.parse();
}