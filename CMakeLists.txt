cmake_minimum_required(VERSION 3.24)
project(course_project)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH /Applications/Development/QT/6.8.0/macos)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(course_project main.cpp src/Interface.cpp src/FileManager.cpp src/Compressor.cpp src/Adapter.cpp include/Interface.h include/Adapter.h include/FileManager.h include/Compressor.h include/Archivator.h src/Archivator.cpp include/InfoManager.h src/InfoManager.cpp)

include_directories(include)

target_link_libraries(course_project Qt6::Widgets)
