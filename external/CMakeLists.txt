# In external repositories do not enfore the pedantic/restrictive compile flags
# Get the COMPILE_OPTIONS property for local scope, remove unnecessary items and
# set the property again
get_property(opt DIRECTORY PROPERTY COMPILE_OPTIONS)
list(REMOVE_ITEM opt "-Wall" "-Wextra" "-Werror" "-Wpedantic" "-pedantic")
set_property(DIRECTORY PROPERTY COMPILE_OPTIONS "${opt}")

set(LIBDEFLATE_BUILD_STATIC_LIB ON CACHE BOOL "Build the static library")
set(LIBDEFLATE_BUILD_SHARED_LIB OFF CACHE BOOL "Build the shared library")
set(LIBDEFLATE_COMPRESSION_SUPPORT OFF CACHE BOOL "Support compression")
set(LIBDEFLATE_DECOMPRESSION_SUPPORT ON CACHE BOOL "Support decompression")
set(LIBDEFLATE_ZLIB_SUPPORT ON CACHE BOOL "Support the zlib format")
set(LIBDEFLATE_GZIP_SUPPORT ON CACHE BOOL "Support the gzip format")
set(LIBDEFLATE_FREESTANDING ON CACHE BOOL
       "Build a freestanding library, i.e. a library that doesn't link to any
       libc functions like malloc(), free(), and memcpy().  Library users will
       need to call libdeflate_set_memory_allocator().")
set(LIBDEFLATE_BUILD_GZIP OFF CACHE BOOL "Build the libdeflate-gzip program")
set(LIBDEFLATE_BUILD_TESTS OFF CACHE BOOL "Build the test programs")
set(LIBDEFLATE_USE_SHARED_LIB OFF CACHE BOOL 
       "Link the libdeflate-gzip and test programs to the shared library instead
       of the static library")

add_subdirectory(libdeflate EXCLUDE_FROM_ALL)
target_include_directories(libdeflate_static PRIVATE ${CMAKE_SOURCE_DIR}/fake_stdlib)