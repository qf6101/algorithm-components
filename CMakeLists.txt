#
# Create by qfeng on 2018-12-14
#

# initialize project
cmake_minimum_required(VERSION 3.13)
project(algorithm-components)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC")

# add flags
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native -pipe")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_PROFILE "-g -O2 -march=native -pipe -D_PROFILE")
set(CMAKE_EXE_LINKER_FLAGS_PROFILE "-lprofiler")
mark_as_advanced(
        CMAKE_CXX_FLAGS_PROFILE
        CMAKE_EXE_LINKER_FLAGS_PROFILE)

# set configuration
set(CMAKE_MODULE_PATH ${PROJECT_BINARY_DIR})
configure_file(ExternallibConfig.cmake.in "${PROJECT_BINARY_DIR}/Findexternallib.cmake" @ONLY)
find_package(externallib REQUIRED)

# set rpath
set(CMAKE_INSTALL_RPATH "\$ORIGIN")

# add sub projects
add_subdirectory(proto)
add_subdirectory(external)
add_subdirectory(core)
add_subdirectory(test)
add_subdirectory(swig)
add_subdirectory(app)
