cmake_minimum_required(VERSION 2.8)
project(ScaraRobot)

# Set the C flags. Only the compiler is set by the platform file. The -mmcu flag is REQUIRED
# by mspgcc to find the correct include files and enable preprocessor defs. this definition should
# be equal to the target board.
set(MSP_CPU_FLAG -mmcu=msp430g2553)
set(CMAKE_C_FLAGS "${MSP_CPU_FLAG}")
set(CMAKE_EXE_LINKER_FLAGS "${MSP_CPU_FLAG}")

# Source Files
set(SOURCES
        src/main.cpp
        src/led.cpp
        src/uart.cpp
        src/pwm.cpp
        src/servo.cpp
    )

# Header files
set(HEADERS
        src/hardware.h
        src/led.h
        src/uart.h
        src/pwm.h
        src/servo.h
    )

# Create the executable
add_executable(firmware.elf ${HEADERS} ${SOURCES})

# Create a flash target if the toolchain file was selected correctly
if (MSP_TOOLCHAIN)
    setup_flash_target(firmware.elf rf2500)
endif ()
