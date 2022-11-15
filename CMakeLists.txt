cmake_minimum_required(VERSION 3.20)
project(vOS C)

set(CMAKE_C_STANDARD 90)


set(SOURCE_FILES 
    
src/core/machine.c
src/core/machine.h
src/core/process_generator.c
src/core/process_generator.h
src/core/timer.c
src/core/timer.h
src/core/clock.c
src/core/clock.h

src/sched/scheduler.c
src/sched/scheduler.h
src/sched/pcb.c
src/sched/pcb.h

src/types.h
src/main.c




)
add_executable(vOS ${SOURCE_FILES})