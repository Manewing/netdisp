add_executable(
  NetDispUnitTest
  TestByteStream.cpp
  TestCommandParser.cpp
  ../../main/src/ByteStream.cpp
  ../../main/src/Parser.cpp
  ../../main/src/CommandBase.cpp
)

target_link_libraries(NetDispUnitTest
  gtest
  gmock
  gtest_main
  pthread
)

target_include_directories(NetDispUnitTest
  PUBLIC ../../main/include
)

target_compile_definitions(NetDispUnitTest
  PUBLIC NETDISP_NO_LOG
)

add_test(
  NAME netdisp_unittest
  COMMAND NetDispUnitTest
)