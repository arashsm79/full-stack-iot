cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS components/)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(esp-sensor)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
    ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
