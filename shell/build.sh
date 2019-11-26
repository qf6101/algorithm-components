#!/bin/bash

#
# Create by qfeng on 2018-12-18
#

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

export CC=/home/machinelearning/platform/gcc-7.3.0/build/bin/gcc
export CXX=/home/machinelearning/platform/gcc-7.3.0/build/bin/g++

if [[ $# -eq 0 ]]
then
    BUILD_DIR=Debug
    BUILD_TYPE=Debug
else
    BUILD_DIR=$1
    BUILD_TYPE=$1
fi

if [[ "$2" = "complete" ]]
then
    rm -rf ${DIR}/../${BUILD_DIR}
    mkdir ${DIR}/../${BUILD_DIR}
fi

if [[ ! -d "$DIR/../${BUILD_DIR}" ]]
then
    mkdir ${DIR}/../${BUILD_DIR}
fi

cd ${DIR}/../${BUILD_DIR}/

cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} && make -j 2
make install