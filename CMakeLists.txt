cmake_minimum_required(VERSION 3.20.0)
project("Emu68 EFI boot" VERSION 0.1.0)

include(cmake/verstring.cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

get_verstring(IDSTRING)

add_subdirectory(external EXCLUDE_FROM_ALL)

add_executable(bootaa64
    main.c
    support.c
)

target_link_libraries(bootaa64 PRIVATE libdeflate_static)
target_include_directories(bootaa64 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(bootaa64 PRIVATE -ffreestanding -Wall -Werror -pedantic -Os)
target_compile_definitions(bootaa64 PRIVATE
    IDSTRING="${IDSTRING}"
)
target_link_options(bootaa64 PRIVATE -subsystem:efi_application -entry:efi_main -nodefaultlib)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/.git/index)

install(PROGRAMS ${CMAKE_BINARY_DIR}/bootaa64.efi DESTINATION ${CMAKE_INSTALL_PREFIX}/efi/boot/)
