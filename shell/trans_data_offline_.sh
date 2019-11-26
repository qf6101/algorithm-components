#!/usr/bin/env bash

#
# Create by qfeng on 2019-01-24
#

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

export CC=/home/machinelearning/platform/gcc-7.3.0/build/bin/gcc
export CXX=/home/machinelearning/platform/gcc-7.3.0/build/bin/g++

outp_option=$1

INP_DIR=$2
OUTP_DIR=${DIR}/../data/${outp_option}/2019
LOG_DIR=${DIR}/../algocomp_logs/app_trans_data
CONF_FILE=${DIR}/../conf/pipeline.xml
EXEC_FILE=${DIR}/../bin/algocomp_trans_data

num_threads=15
bs=5000

if [[ ! -e "${OUTP_DIR}/vocab"  ]]
then
    echo "Vocab directory does not exist, will be created."
    mkdir -p ${OUTP_DIR}/vocab
fi

if [[ ! -e "${OUTP_DIR}/encoded" ]]
then
    echo "Encoded directory does not exist, will be created."
    mkdir -p ${OUTP_DIR}/encoded
fi

for month_ in ${INP_DIR}/*
do
	month=`basename ${month_}`
	if [[ -d "${OUTP_DIR}/encoded/${month}" ]]
	then
		echo "Output directory of month "${month}" already exists, will be clear."
		rm ${OUTP_DIR}/encoded/${month}/*
		rm ${OUTP_DIR}/vocab/${month}.vocab
	else
		echo "Output directory of month "${month}" does not exist yet, will be created."
		mkdir -p ${OUTP_DIR}/encoded/${month}
	fi

	echo "Start processing month: ==> ${month}"

	${EXEC_FILE} -t attrs_to_encs -l ${LOG_DIR}/${month} \
		-c ${CONF_FILE} -p "pl-1" -s ${OUTP_DIR}/vocab/"${month}.vocab" \
		-i ${INP_DIR}/${month} -o ${OUTP_DIR}/encoded/${month} -n ${num_threads} -b ${bs}

	echo "Finish processing month: ==> ${month}"
done
