add_executable(
  NetDispUnitTest
  TestByteStream.cpp
  ../../main/src/ByteStream.cpp
)

target_link_libraries(NetDispUnitTest
  gtest
  gtest_main
  pthread
)

target_include_directories(NetDispUnitTest
  PUBLIC ../../main/include
)

add_test(
  NAME netdisp_unittest
  COMMAND NetDispUnitTest
)