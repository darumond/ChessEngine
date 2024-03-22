cmake_minimum_required(VERSION 3.21)
project(Chess)

set(CMAKE_CXX_STANDARD 20)

add_executable(chessengine
        src/main.cc
        src/Board/board.cc
        src/Pieces/pawn.cc
        src/Pieces/rook.cc
        src/Pieces/knight.cc
        src/Pieces/king.cc
        src/Pieces/queen.cc
        src/Pieces/bishop.cc
        src/AI/uci.cc)

add_library(compile_options INTERFACE)

target_compile_options(compile_options INTERFACE
        "-Wall"
        "-Wextra"
        "-Werror"
        "-pedantic"
        "-fno-builtin"
        "-fsanitize=address"
        "-g"
        )

target_link_options(chessengine PRIVATE
        "-fsanitize=address"
        )

set_target_properties(chessengine PROPERTIES
        CXX_STANDARD 20
        CXX_EXTENSIONS OFF
        )

#
#add_executable(FooTest test/testsuite.cc)
#
#enable_testing()
#find_package(GTest REQUIRED)
#include_directories("/usr/local/include")
#
#
#link_libraries("/usr/local/lib/libgtest.a")
#
#
#gtest_add_tests(TARGET FooTest
#        TEST_LIST noArgsTests
#        )

include_directories(chessengine PUBLIC src/Pieces src/Board src/AI)

target_link_libraries(chessengine PRIVATE compile_options)

add_custom_target(build_all DEPENDS chessengine)
