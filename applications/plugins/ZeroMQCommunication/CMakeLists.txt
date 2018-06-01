cmake_minimum_required(VERSION 2.8.12)
project(ZeroMQCommunication)

set(${PROJECT_NAME}_VERSION 1.0)

set(HEADER_FILES
    initZeroMQCommunication.h
    ZMQClientComponent.h
    SerialDriver.h
    SurgicalProtocol.h
    SofaTypeMessages.h
)

set(SOURCE_FILES
    initZeroMQCommunication.cpp
    ZMQClientComponent.cpp
    SerialDriver.cpp
    SurgicalProtocol.cpp
    SofaTypeMessages.cpp
)

set(EXTRA_FILES
    README.md
)

find_package(SofaFramework REQUIRED)
find_package(SofaBase REQUIRED)
find_package(SofaGeneral REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_ZEROMQCOMMUNICATION")

# Added
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_SERIALPLUGIN")

target_link_libraries(${PROJECT_NAME} -lzmq ${} SofaCore SofaComponentGeneral)


## Install rules for the library; CMake package configurations files
sofa_create_package(${PROJECT_NAME} ${${PROJECT_NAME}_VERSION} ${PROJECT_NAME} ${PROJECT_NAME})

install(DIRECTORY examples/ DESTINATION share/sofa/plugins/${PROJECT_NAME})