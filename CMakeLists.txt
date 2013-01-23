cmake_minimum_required(VERSION 2.6)

project(ZELOSONLINE)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/FindMySQL.cmake)

add_subdirectory (blowfish)
add_subdirectory (libpacket)
add_subdirectory (libauth)
add_subdirectory (libnet)
add_subdirectory (libname)
add_subdirectory (libdb)
add_subdirectory (libkhamaileon)
add_subdirectory (libpos)
add_subdirectory (libsso)
add_subdirectory (libphysic)

add_subdirectory (certd)
add_subdirectory (globald)
add_subdirectory (gatewayd)
add_subdirectory (named)
add_subdirectory (shardd)
add_subdirectory (agentd)
add_subdirectory (gamed)

add_subdirectory (services)

