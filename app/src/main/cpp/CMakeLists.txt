cmake_minimum_required(VERSION 3.18.1)
project(AsanTest)

find_library(LIB_LOG log)

add_library(app SHARED app.cpp)
target_compile_options(app PUBLIC -fsanitize=address -fno-omit-frame-pointer)
set_target_properties(app PROPERTIES LINK_FLAGS -fsanitize=address)
target_link_libraries(app ${LIB_LOG})

get_filename_component(HINT_PATH ${ANDROID_C_COMPILER} DIRECTORY)
set(ARCH_STR ${CMAKE_ANDROID_ARCH})
if ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "arm64-v8a")
    set(ARCH_STR "aarch64")
elseif ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "armeabi")
    set(ARCH_STR "arm")
elseif ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "armeabi-v7a")
    set(ARCH_STR "arm")
elseif ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "x86")
    set(ARCH_STR "i686")
elseif ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "x86_64")
    set(ARCH_STR "x86_64")
endif()

# TODO: Figure out how to wildcard the Clang version.
find_file(ASAN
        NAMES libclang_rt.asan-${ARCH_STR}-android.so
        HINTS ${HINT_PATH}/../lib64/clang/14.0.1/lib/linux)

get_filename_component(ASAN_NAME ${ASAN} NAME)
set(ASAN_NAME ${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}/${ASAN_NAME})

add_custom_command(
        TARGET app PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${ASAN} ${ASAN_NAME})
