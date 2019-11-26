#
# Create by qfeng on 2018-12-14
#

# include head files
include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/external/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/external/vw_hash/include)
include_directories(${PROJECT_SOURCE_DIR}/external/sparsehash/include)
include_directories(${PROJECT_SOURCE_DIR}/external/CTPL/include)
include_directories(${PROJECT_SOURCE_DIR}/external/abseil-cpp)
include_directories(${PROJECT_SOURCE_DIR}/external/base64)
include_directories(${PROJECT_SOURCE_DIR}/external/adaqs/include)
include_directories(${PROJECT_SOURCE_DIR}/external/dmlc/include)
include_directories(${PROJECT_SOURCE_DIR}/external/xgboost-predictor/include)
include_directories(${PROJECT_SOURCE_DIR}/proto)
# construct source files
file(GLOB src_files "src/*/*.cpp")
set(SOURCE_FILES ${src_files})
# add source files to this dynamic library
add_library(algocomp_core SHARED ${SOURCE_FILES})
target_link_libraries(algocomp_core
        algocomp_proto
        vw_hash
        absl_strings
        boost_system
        boost_filesystem
        boost_chrono
        boost_regex
        boost_thread
        adaqs)
# install this target
install(TARGETS algocomp_core DESTINATION ${PROJECT_BINARY_DIR}/bin)
