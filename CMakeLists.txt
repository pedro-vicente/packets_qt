cmake_minimum_required(VERSION 3.28)
project(network_qt)
add_subdirectory(packets)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT packets)
