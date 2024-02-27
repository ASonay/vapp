
cmake_minimum_required(VERSION 3.10)

project(VisualApp)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Widgets Core 3DCore 3DRender 3DInput 3DExtras REQUIRED)

# Automatically find and add subdirectories that contain a CMakeLists.txt file
file(GLOB children RELATIVE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/*/*)
foreach(child ${children})
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${child} AND EXISTS ${CMAKE_SOURCE_DIR}/${child}/CMakeLists.txt)
        message(STATUS "Adding subdirectory: ${child}")
        add_subdirectory(${child})
    endif()
endforeach()

# Enable AUTOMOC
set_target_properties(QtManager PROPERTIES AUTOMOC ON)

add_executable(vapp App/main.cpp)

target_link_libraries(vapp Config AlgoBase QtManager)