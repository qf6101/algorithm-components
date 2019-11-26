# include head files
include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/external/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/external/sparsehash/include)
# construct source files
file(GLOB src_files "src/*/*.cpp")
set(SOURCE_FILES ${src_files})
# add source files to this dynamic library
add_library(adaqs SHARED ${SOURCE_FILES})
# install this target
install (TARGETS adaqs DESTINATION ${PROJECT_BINARY_DIR}/bin)
