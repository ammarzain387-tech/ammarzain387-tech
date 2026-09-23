cmake_minimum_required(VERSION 3.16)
project(cpp_security_analyzer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(security-analyzer src/main.cpp)
target_compile_options(security-analyzer PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

include(GNUInstallDirs)
install(TARGETS security-analyzer RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

option(ENABLE_TESTS "Build tests" OFF)
if(ENABLE_TESTS)
    enable_testing()
endif()
