# include head files
include_directories(.)
# construct source files
file(GLOB src_files "*/*.cc")
set(SOURCE_FILES ${src_files})
# add source files to this dynamic library
add_library(algocomp_proto SHARED ${SOURCE_FILES})
target_link_libraries(algocomp_proto protobuf)
# install this target
install (TARGETS algocomp_proto DESTINATION ${PROJECT_BINARY_DIR}/bin)
