#
# Create by qfeng on 2018-12-14
#
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHON_LIBRARY_DIRS})

# include head files and link libraries
include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/proto)
include_directories(${PROJECT_SOURCE_DIR}/core/include)
include_directories(${PROJECT_SOURCE_DIR}/external/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/external/googletest/googletest/include)
include_directories(${PROJECT_SOURCE_DIR}/external/vw_hash/include)
include_directories(${PROJECT_SOURCE_DIR}/external/sparsehash/include)
include_directories(${PROJECT_SOURCE_DIR}/external/CTPL/include)
include_directories(${PROJECT_SOURCE_DIR}/external/abseil-cpp)
include_directories(${PROJECT_SOURCE_DIR}/external/base64)

# add test executable
file(GLOB src_files "src/*/*.cpp")
set(SOURCE_FILES ${src_files} src/main.cpp)
add_executable(algocomp_test ${SOURCE_FILES})
target_link_libraries(algocomp_test
        algocomp_proto
        algocomp_core
        vw_hash
        boost_thread
        boost_system
        pthread
        boost_filesystem
        boost_regex
        protobuf
        gtest
        gtest_main
        ${PYTHON_LIBRARIES})
