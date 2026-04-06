# cmake/toolchain-aarch64-efi.cmake

# Find clang by looking for versioned and unversioned names
find_program(CMAKE_C_COMPILER
    NAMES clang-19 clang-18 clang-17 clang-16 clang-15 clang-14 clang-13
    REQUIRED
)
find_program(CMAKE_CXX_COMPILER
    NAMES clang++-19 clang++-18 clang++-17 clang++-16 clang++-15 clang++-14 clang++-13
    REQUIRED
)

# Derive LLVM bin dir from wherever clang was found, so all tools match the same version
get_filename_component(LLVM_BIN_DIR ${CMAKE_C_COMPILER} DIRECTORY)

# Try to extract version suffix from the found clang binary name
get_filename_component(CLANG_BINARY ${CMAKE_C_COMPILER} NAME)
string(REGEX MATCH "-[0-9]+$" LLVM_VERSION_SUFFIX ${CLANG_BINARY})

find_program(CMAKE_AR
    NAMES llvm-lib${LLVM_VERSION_SUFFIX} llvm-lib
    HINTS ${LLVM_BIN_DIR}
    REQUIRED
)

find_program(CMAKE_LINKER
    NAMES lld-link${LLVM_VERSION_SUFFIX} lld-link
    HINTS ${LLVM_BIN_DIR}
    REQUIRED
)

set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_LIST_DIR}/msvc-overrides.cmake)

set(CMAKE_SYSTEM_NAME      Windows)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Skip compiler sanity checks - they fail in freestanding cross-compilation
set(CMAKE_C_COMPILER_WORKS   1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# These _INIT variables seed the cache before any CMakeLists.txt runs,
# including subprojects - this is the correct mechanism for toolchain-wide flags
set(CMAKE_C_FLAGS_INIT   "-target aarch64-unknown-windows -mno-red-zone")
set(CMAKE_CXX_FLAGS_INIT "-target aarch64-unknown-windows -mno-red-zone")

# ranlib is a no-op with llvm-lib
find_program(CMAKE_RANLIB true REQUIRED)
find_program(CMAKE_RC_COMPILER true REQUIRED)

# Static lib extension and no lib prefix, matching MSVC conventions
set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
