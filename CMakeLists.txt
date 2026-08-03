cmake_minimum_required(VERSION 3.10)

project(AMD-CPUID LANGUAGES C CXX)

set(SOURCES
    src/main.cpp
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND SOURCES src/amd/linux.cpp)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND SOURCES src/amd/windows.cpp)
else()
    message(WARNING "Unsupported operating system: ${CMAKE_SYSTEM_NAME}. Build might fail.")
endif()

add_executable(amd-cpuid ${SOURCES})

target_include_directories(amd-cpuid PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/amd
)

set_target_properties(amd-cpuid PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED YES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
)