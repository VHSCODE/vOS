cmake_minimum_required(VERSION 3.20)
project(vOS C)

set(CMAKE_C_STANDARD 90)


set(SOURCE_FILES

        src/Machine.c
        src/Machine.h
        src/core/Loader.c
        src/core/Loader.h
        src/core/Timer.c
        src/core/Timer.h
        src/core/Clock.c
        src/core/Clock.h
src/core/CPU.c
        src/core/CPU.h


        src/sched/scheduler.c
src/sched/scheduler.h
src/sched/pcb.c
src/sched/pcb.h

src/types.h
src/main.c




)
add_executable(vOS ${SOURCE_FILES})