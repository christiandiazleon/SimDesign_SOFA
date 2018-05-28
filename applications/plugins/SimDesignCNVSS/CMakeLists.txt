cmake_minimum_required(VERSION 3.1)
project(SimDesignCNVSS)


set(HEADER_FILES
    initSimDesignCNVSS.h
    CNVSSTesting.h
    NetworkControllerCNVSS.h
    TetrahedralCorotationalFEMForceFieldCNVSS.h
    TetrahedralCorotationalFEMForceFieldCNVSS.inl
    PointNet.h
    MeshLoaderCNVSS.h
    MeshGmshLoaderCNVSS.h
)

set(SOURCE_FILES
    initSimDesignCNVSS.cpp
    CNVSSTesting.cpp
    NetworkControllerCNVSS.cpp
    TetrahedralCorotationalFEMForceFieldCNVSS.cpp
    PointNet.cpp
    MeshLoaderCNVSS.cpp
    MeshGmshLoaderCNVSS.cpp
)

find_package(SofaFramework REQUIRED)
find_package(SofaBase REQUIRED)
find_package(SofaGeneral REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} SofaCore SofaOpenglVisual SofaComponentGeneral SofaGeneralLoader)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_SIMDESIGNCNVSS")

install(TARGETS SimDesignCNVSS
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
