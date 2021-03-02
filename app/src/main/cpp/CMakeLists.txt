# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("greyseal" CXX C)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(
        headers
        ntt/include
)

add_library(seal-ntt SHARED ntt/ntt.c ntt/ntte.c ntt/nttnum.c)

# link_directories(${CMAKE_BINARY_DIR}/cpp)
target_link_libraries(
        seal-ntt
)

add_library( # Sets the name of the library.
        greyseal-core

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        seal/gl_core.cpp
        seal/mesh.cpp
        seal/material.cpp
        seal/texture.cpp
        gs_android/gs_assets.cpp
        seal/scene.cpp
        seal/seal_calls.cpp
        seal/preset.cpp
        seal/bytestream.cpp
        seal/jseal.cpp

        native-lib.cpp )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
add_dependencies(greyseal-core seal-ntt)


find_library( # Sets the name of the path variable.
        log-lib
        log
        )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries(
        seal-ntt

        ${log-lib}
)

target_link_libraries( # Specifies the target library.
        greyseal-core

        # Links the target library to the log library
        # included in the NDK.
        seal-ntt
        android
        GLESv2
        ${log-lib} )