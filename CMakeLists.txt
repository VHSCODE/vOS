cmake_minimum_required(VERSION 3.20)
project(vOS C)

set(CMAKE_C_STANDARD 90)

add_executable(vOS main.c machine.c machine.h types.h  clock.h clock.c process_generator.c process_generator.h timer.c timer.h pcb.h pcb.c)
