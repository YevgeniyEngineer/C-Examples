cmake_minimum_required(VERSION 3.18)

project(c_examples)

set(C_STANDARD 17)
set(C_STANDARD_REQUIRED ON)

add_executable(vector ./examples/vector.c)
