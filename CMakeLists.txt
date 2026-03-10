cmake_minimum_required(VERSION 3.20)
project(ProjectVortex VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets WebEngineWidgets WebChannel)

add_executable(vortex
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/ImageController.cpp
    src/ImageController.h
    src/ModuleManager.cpp
    src/ModuleManager.h
    src/module_api.h
    resources.qrc
)

target_link_libraries(vortex PRIVATE
    Qt6::Widgets
    Qt6::WebEngineWidgets
    Qt6::WebChannel
)

target_include_directories(vortex PRIVATE src)

add_library(sample_module SHARED modules/sample_invert_module.cpp)
target_include_directories(sample_module PRIVATE src)
set_target_properties(sample_module PROPERTIES
    OUTPUT_NAME "sample_invert_module"
)

install(TARGETS vortex RUNTIME DESTINATION bin)
install(TARGETS sample_module LIBRARY DESTINATION modules RUNTIME DESTINATION modules)
