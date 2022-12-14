cmake_minimum_required(VERSION 3.20)
project(vOS C)

set(CMAKE_C_STANDARD 90)


set(SOURCE_FILES

        src/Machine.c
        src/Machine.h
        src/core/Memory/Loader.c
        src/core/Memory/Loader.h
        src/core/CPU/Timer.c
        src/core/CPU/Timer.h
        src/core/CPU/Clock.c
        src/core/CPU/Clock.h
        src/core/CPU/CPU.c
        src/core/CPU/CPU.h
        src/sched/scheduler.c
        src/sched/scheduler.h
        src/sched/pcb.c
        src/sched/pcb.h

        src/types.h
        src/main.c


        src/core/Memory/MMU.h src/core/CPU/Instructions.h src/core/CPU/Instructions.c)
add_executable(vOS ${SOURCE_FILES})