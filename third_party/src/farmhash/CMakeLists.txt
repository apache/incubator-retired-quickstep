include(CheckCXXSourceCompiles)

# Compile FarmHash with C++11 support.
set_property(
  DIRECTORY
  APPEND PROPERTY COMPILE_DEFINITIONS FARMHASH_CAN_USE_CXX11
)

# Check if __builtin_expect is available.
CHECK_CXX_SOURCE_COMPILES("
  int main(int argc, char **argv) {
    if (__builtin_expect(argc, 1)) {
      return 0;
    } else {
      return argc;
    }
  }
" HAVE_BUILTIN_EXPECT)
if (NOT HAVE_BUILTIN_EXPECT)
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS FARMHASH_NO_BUILTIN_EXPECT
  )
endif()

# Check endianness.
include(TestBigEndian)
TEST_BIG_ENDIAN(WORDS_BIGENDIAN)
if (WORDS_BIGENDIAN)
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS WORDS_BIGENDIAN
  )
endif()

# TODO(chasseur): FarmHash conditionally compiles fast versions of hash
# functions that use SSSE3, SSE4.1, SSE4.2, AES-NI, and AVX, where available,
# which are turned on based on preprocessor macros defined by GCC and other
# similar compilers (e.g. Clang, ICC) when those instructions sets are usable.
# MSVC sets __AVX__ when AVX is available, but does NOT set symbols for any
# other instruction set extensions optionally used by FarmHash, so building
# with MSVC will fall back on generic x86 versions of some internal hashing
# functions. Try to work out a portable way of detecting the other extensions
# and enabling them under MSVC.

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(farmhash farmhash.cc)

# Supress GCC warnings about overflow in farmhash only.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-Wno-overflow" COMPILER_HAS_WNO_OVERFLOW)
if (COMPILER_HAS_WNO_OVERFLOW)
  set_target_properties(farmhash PROPERTIES COMPILE_FLAGS "-Wno-overflow")
endif()
