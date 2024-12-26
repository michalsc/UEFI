cmake_minimum_required(VERSION 3.20.0)
project(EFI-boot VERSION 0.1.0)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

find_program(LLD_EXECUTABLE NAMES lld-link REQUIRED)

set(CMAKE_C_LINK_EXECUTABLE "${LLD_EXECUTABLE} <LINK_FLAGS> <OBJECTS> -out:<TARGET>")
set(CMAKE_CXX_LINK_EXECUTABLE "${LLD_EXECUTABLE} <LINK_FLAGS> <OBJECTS> -out:<TARGET>")

add_executable(test.efi
    main.c
    support.c
)

target_include_directories(test.efi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(test.efi PRIVATE -Os -ffreestanding -mno-red-zone -target aarch64-unknown-windows -Wall -Werror -pedantic)
target_link_options(test.efi PRIVATE -subsystem:efi_application -entry:efi_main)
