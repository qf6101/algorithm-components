#
# Create by qfeng on 2018-12-14
#

# add swig and java supports
cmake_policy(SET CMP0078 NEW)
find_package(SWIG REQUIRED)
find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)
include(UseSWIG)
include(${SWIG_USE_FILE})
include_directories(${JNI_INCLUDE_DIRS})
set(CMAKE_JAVA_COMPILE_FLAGS "-source" "1.8" "-target" "1.8")
enable_testing()
# configure swig properties (avoiding using -includeall)
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_SWIG_FLAGS -package algocomp.swig)
set_source_files_properties(algocomp.i PROPERTIES CPLUSPLUS ON)
# add head files
include_directories(${PROJECT_SOURCE_DIR}/core/include)
include_directories(${PROJECT_SOURCE_DIR}/external/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/external/CTPL/include)
include_directories(${PROJECT_SOURCE_DIR}/external/sparsehash/include)
include_directories(${PROJECT_SOURCE_DIR}/proto)

# add swig files
swig_add_library(
        algocomp_swig
        TYPE SHARED
        LANGUAGE java
        SOURCES algocomp.i
)
# link to core sub projects
SWIG_LINK_LIBRARIES(algocomp_swig algocomp_core algocomp_proto)
# install this target
install (TARGETS algocomp_swig DESTINATION ${PROJECT_BINARY_DIR}/bin)
file(GLOB java_src ${PROJECT_BINARY_DIR}/swig/*.java)
install (FILES ${java_src} DESTINATION ${PROJECT_BINARY_DIR}/java)