cmake_minimum_required(VERSION 3.1)
project(SimDesignCNVSS)


set(HEADER_FILES
    initSimDesignCNVSS.h
    CNVSSTesting.h
)

set(SOURCE_FILES
    initSimDesignCNVSS.cpp
    CNVSSTesting.cpp
)

find_package(SofaFramework REQUIRED)
find_package(SofaBase REQUIRED)
find_package(SofaGeneral REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} SofaCore SofaOpenglVisual)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_SIMDESIGNCNVSS")

install(TARGETS SimDesignCNVSS
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
