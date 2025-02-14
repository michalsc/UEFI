# In external repositories do not enfore the pedantic/restrictive compile flags
# Get the COMPILE_OPTIONS property for local scope, remove unnecessary items and
# set the property again
get_property(opt DIRECTORY PROPERTY COMPILE_OPTIONS)
list(REMOVE_ITEM opt "-Wall" "-Wextra" "-Werror" "-Wpedantic" "-pedantic")
set_property(DIRECTORY PROPERTY COMPILE_OPTIONS "${opt}")

option(LIBDEFLATE_BUILD_STATIC_LIB "" ON)
option(LIBDEFLATE_BUILD_SHARED_LIB "" OFF)
option(LIBDEFLATE_COMPRESSION_SUPPORT "" OFF)
option(LIBDEFLATE_DECOMPRESSION_SUPPORT "" ON)
option(LIBDEFLATE_ZLIB_SUPPORT "" ON)
option(LIBDEFLATE_GZIP_SUPPORT "" ON)
option(LIBDEFLATE_FREESTANDING "" ON)
option(LIBDEFLATE_BUILD_GZIP "" OFF)
option(LIBDEFLATE_BUILD_TESTS "" OFF)
option(LIBDEFLATE_USE_SHARED_LIB "" OFF)

add_subdirectory(libdeflate)
target_include_directories(libdeflate_static PRIVATE ${CMAKE_SOURCE_DIR}/fake_stdlib)
