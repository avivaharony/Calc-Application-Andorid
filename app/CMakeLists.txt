# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.

cmake_minimum_required(VERSION 3.4.1)

# Specifies a library name, specifies whether the library is STATIC or
# SHARED, and provides relative paths to the source code. You can
# define multiple libraries by adding multiple add_library() commands,
# and CMake builds them for you. When you build your app, Gradle
# automatically packages shared libraries with your APK.

add_library( # Specifies the name of the library.
             calc-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/stack.c
             src/main/cpp/calc_with_power.c
           #  src/main/cpp/calc_with_power_test.c
            src/main/cpp/hello-jni.c
            src/main/cpp/calc-jni.c
        )

#add_executable(calc_exec_test.out
#        src/main/cpp/calc_with_power_test.c
#        )


# Specifies a path to native header files.
include_directories(src/main/cpp/include/)
#include_directories(src/main/jni/)