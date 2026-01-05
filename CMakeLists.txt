cmake_minimum_required (VERSION 3.14)

project ("ImGuiAX" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/Dependencies.cmake)

add_subdirectory("libs")
add_subdirectory("demo")
