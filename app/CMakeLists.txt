# include head files and link libraries
include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/proto)
include_directories(${PROJECT_SOURCE_DIR}/core/include)
include_directories(${PROJECT_SOURCE_DIR}/external/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/external/vw_hash/include)
include_directories(${PROJECT_SOURCE_DIR}/external/sparsehash/include)
include_directories(${PROJECT_SOURCE_DIR}/external/CTPL/include)
include_directories(${PROJECT_SOURCE_DIR}/external/abseil-cpp)
include_directories(${PROJECT_SOURCE_DIR}/external/base64)
# add profile executable
add_executable(algocomp_profile
        src/profile/algocomp_profile.cpp)
target_link_libraries(algocomp_profile
        algocomp_proto
        algocomp_core
        vw_hash
        boost_thread
        boost_system
        pthread
        boost_filesystem
        protobuf
        )

IF (CMAKE_BUILD_TYPE MATCHES Profile)
    target_link_libraries(algocomp_profile profiler)
ENDIF (CMAKE_BUILD_TYPE MATCHES Profile)

# add eval executables
add_executable(algocomp_eval_time
        src/eval_time/algocomp_eval_time.cpp)
target_link_libraries(algocomp_eval_time
        algocomp_proto
        algocomp_core
        vw_hash
        boost_thread
        boost_system
        pthread
        boost_filesystem
        protobuf
        )

add_executable(algocomp_trans_data
        src/trans_data/data_reader.cpp
        src/trans_data/data_writer.cpp
        src/trans_data/trans_data_offline.cpp)
target_link_libraries(algocomp_trans_data
        algocomp_proto
        algocomp_core
        boost_program_options
        vw_hash
        boost_thread
        boost_system
        pthread
        boost_filesystem
        boost_chrono
        protobuf)

# create log directory for spdlog
add_custom_command(TARGET algocomp_profile PRE_BUILD
        COMMAND rm -rf ${PROJECT_SOURCE_DIR}/algocomp_logs/app
        COMMAND mkdir -p ${PROJECT_SOURCE_DIR}/algocomp_logs/app)

# install targets
install(TARGETS algocomp_profile DESTINATION ${PROJECT_BINARY_DIR}/bin)
install(TARGETS algocomp_eval_time DESTINATION ${PROJECT_BINARY_DIR}/bin)
install(TARGETS algocomp_trans_data DESTINATION ${PROJECT_BINARY_DIR}/bin)