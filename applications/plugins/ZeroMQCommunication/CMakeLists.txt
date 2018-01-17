cmake_minimum_required(VERSION 3.1)
project(ZeroMQCommunication)

set(HEADER_FILES
    initZeroMQCommunication.h
    PointNet.h
    ZMQServerComponent.h
    SofaTypeMessages.h
    Client.h
)

set(SOURCE_FILES
    initZeroMQCommunication.cpp
    PointNet.cpp
    ZMQServerComponent.cpp
    SofaTypeMessages.cpp
    Client.cpp   
)

set(EXTRA_FILES
    README.md
)

find_package(SofaFramework REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
target_link_libraries(${PROJECT_NAME} -lzmq ${} SofaCore)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_ZeroMQCommunication")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

install(TARGETS ZeroMQCommunication
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
