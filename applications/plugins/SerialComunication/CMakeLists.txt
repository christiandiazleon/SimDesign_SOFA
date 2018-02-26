cmake_minimum_required(VERSION 2.8.12)
project(SerialComunication)

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

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(${PORJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_SERIALPLUGIN")
target_link_libraries(${PROJECT_NAME} SofaCore SofaComponentGeneral)

install(TARGETS ${PROJECT_NAME}
		COMPONENT SerialComunication_libraries
		EXPORT SerialComunicationTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY examples/ DESTINATION share/sofa/plugins/${PROJECT_NAME})