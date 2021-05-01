# CMakeList.txt : CMake project for chessbot, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("chessbot")

# Add source to this project's executable.
add_executable (chessbot "chessbot.cpp" "chessbot.h" "board.cpp" "board.h")

# TODO: Add tests and install targets if needed.
