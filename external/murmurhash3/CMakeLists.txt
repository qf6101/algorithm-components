# include head files
include_directories(include)
# construct source files
file(GLOB src_files "src/*.cpp")
set(SOURCE_FILES ${src_files})
# add source files to this dynamic library
add_library(murmurhash3 SHARED ${SOURCE_FILES})
# install this target
install (TARGETS murmurhash3 DESTINATION ${PROJECT_BINARY_DIR}/bin)
