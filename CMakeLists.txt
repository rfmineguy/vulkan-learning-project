cmake_minimum_required(VERSION 3.2)

project (vk_demo LANGUAGES CXX C)

#custom cmake option

#general cmake setting configuration
set (CMAKE_CXX_STANDARD_REQUIRED True)
set (CMAKE_EXPORT_COMPILE_COMMANDS True)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Vulkan REQUIRED FATAL_ERROR)

set (SOURCES 
    src/main.cpp
    src/helloTriangleApp.h
    src/helloTriangleApp.cpp)


include_directories(
    src/
    vendor/
    vendor/glfw/include
    ${Vulkan_INCLUDE_DIRS})

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)

add_executable(vk_demo ${SOURCES})
target_link_libraries(vk_demo
    glfw
    glm
    ${Vulkan_LIBRARIES})
