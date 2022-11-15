cmake_minimum_required(VERSION 3.23)
project(compiler)

set(CMAKE_CXX_STANDARD 23)

add_executable(compiler main.cpp LexicalAnalyser.cpp LexicalAnalyser.h Automatic.cpp Automatic.h Types.h Grammar.cpp Grammar.h)
