cmake_minimum_required(VERSION 3.8)
project(Diff3_0)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Differentiator/main.cpp Tree_t/Tree.cpp Tree_t/Tree.h "Differentiator/Differentiator .cpp" "Differentiator/Differentiator .h" RecursiveDescent/MathFunc.h RecursiveDescent/Descent.cpp RecursiveDescent/Descent.h Commands/Commands.h)
add_executable(Diff3_0 ${SOURCE_FILES} Tree_t/Tree.cpp Tree_t/Tree.h)