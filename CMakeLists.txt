cmake_minimum_required(VERSION 3.14.1)
project(uwpspeech)


if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR OR EXISTS "${CMAKE_BINARY_DIR}/CMakeLists.txt")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif()

if(NOT WIN32)
  message(FATAL "This project targets Windows")
endif()

# This setting targets VS2019. Change metadata1 and metadata2 according VS version.
set(metadata1 "C:\\Program Files (x86)\\Windows Kits\\10\\UnionMetadata\\10.0.18362.0")
set(metadata2 "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\VC\\vcpackages")
set(CMAKE_CXX_STANDARD 17)
set(
  SOURCES
  src/main.cpp
)

add_compile_options("/ZW")
add_executable(uwpspeech ${SOURCES})
target_compile_options(uwpspeech PRIVATE $<$<BOOL:${MSVC}>:/AI${metadata1} /AI${metadata2}>)
