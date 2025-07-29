cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)
qt_standard_project_setup()

#//////////////////////////
# Qt
# link with libraries
# lib_dep contains a cascade definition of all the libraries needed to link
#//////////////////////////

set(lib_dep ${lib_dep})
set(lib_dep ${lib_dep} Qt6::Widgets)
set(lib_dep ${lib_dep} Qt6::Network)
set(lib_dep ${lib_dep} Qt6::Core)

set(src ${src})
set(src ${src} main.cc)
set(src ${src} packets.cc)
set(src ${src} packets.h)
set(src ${src} window.cc)
set(src ${src} window.h)


add_executable(packets ${src})
target_link_libraries(packets PRIVATE ${lib_dep})
set_target_properties(packets PROPERTIES WIN32_EXECUTABLE ON MACOSX_BUNDLE ON)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT packets)
