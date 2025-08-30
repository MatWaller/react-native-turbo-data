cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options(-fexceptions -frtti -std=c++17 -Wall -DLOG_TAG=\"ReactNative\")

file(GLOB turbodata_SRC CONFIGURE_DEPENDS *.cpp)
add_library(turbodata
        SHARED
        ${turbodata_SRC}
)

target_include_directories(turbodata PUBLIC .)

find_package(ReactAndroid REQUIRED CONFIG)
find_package(fbjni REQUIRED CONFIG)

target_link_libraries(turbodata
        ReactAndroid::jsi
        ReactAndroid::reactnativejni
        fbjni::fbjni
        android
)
