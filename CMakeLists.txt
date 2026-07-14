cmake_minimum_required(VERSION 3.12)

project(ustr VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(ustr STATIC
    src/ustr.cc
    src/unicode_script.cc
)

target_include_directories(ustr
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(ustr_test
    src/main.cc
    src/test.cc
    src/ustr_test.cc
)

target_link_libraries(ustr_test PRIVATE ustr)

enable_testing()
add_test(NAME ustr_test COMMAND ustr_test)
