cmake_minimum_required(VERSION 3.8)
project(Program.elf)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.c Tree_t/Tree.c Tree_t/Tree.h "Differentiator/Differentiator.c" "Differentiator/Differentiator.h" Commands/MathFunc.h RecursiveDescent/Descent.c RecursiveDescent/Descent.h Commands/Commands.h Differentiator/List.h)
add_executable(Program.elf ${SOURCE_FILES} Tree_t/Tree.c Tree_t/Tree.h)

set(GCC_COVERAGE_COMPILE_FLAGS "-lm")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )