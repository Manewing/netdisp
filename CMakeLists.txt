# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/components)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -O3"
)

set(
  PYTHON_OUT_DIR
  ${CMAKE_BINARY_DIR}/lib/python3/dist-packages/netdisp
)
set(
  PYTHON_OUT_INSTALL_DIR
  lib/python3/dist-packages/netdisp
)

add_subdirectory(python3)

add_custom_command(
  OUTPUT
  ${CMAKE_CURRENT_BINARY_DIR}/exports
  DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/exports
  COMMAND
  cp -a ${CMAKE_CURRENT_SOURCE_DIR}/exports ${CMAKE_CURRENT_BINARY_DIR}/exports
)

add_custom_target(
  copy_exports
  ALL
  DEPENDS
  exports
)

project(netdisp)