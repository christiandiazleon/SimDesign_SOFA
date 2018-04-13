cmake_minimum_required(VERSION 2.8.12)
project(SerialComunication)

set(${PROJECT_NAME}_VERSION 1.0)

set(HEADER_FILES
	SerialDriver.h
)

set(SOURCE_FILES
	SerialDriver.cpp
	initSerial.cpp
)

find_package(SofaFramework REQUIRED)
find_package(SofaBase REQUIRED)
find_package(SofaGeneral REQUIRED)
find_package(ZeroMQCommunication REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_SERIALPLUGIN")
target_link_libraries(${PROJECT_NAME} SofaCore SofaComponentGeneral)

##Include targets ZMQCommunication 
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${ZeroMQCommunication_INCLUDE_DIRS}>")
target_link_libraries(${PROJECT_NAME} ZeroMQCommunication)

## Install rules for the library; CMake package configurations files
##sofa_create_package(${PROJECT_NAME} ${${PROJECT_NAME}_VERSION} ${PROJECT_NAME} ${PROJECT_NAME})

install(DIRECTORY examples/ DESTINATION share/sofa/plugins/${PROJECT_NAME})