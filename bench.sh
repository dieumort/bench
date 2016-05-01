#!/bin/bash

#-------------------------------------------------------------------------------

# parameters
TIMEOUT=5
INIT_SCALE=1
INIT_BASE=1000000
MAX_SIZE=90000000

# executions
CPPMAIN=main.cpp
CPPOUT=cpp.bench

# update
if [ ${CPPOUT} -ot ${CPPMAIN} ]; then
    g++ -O3 -std=c++11 ${CPPMAIN} -o ${CPPOUT}
fi

# execute function
# $1: execute command (ex. ./a.out)
function execute() {
    START=`date "+%s"`
    TIME=${TIMEOUT}
    SCALE=${INIT_SCALE}
    BASE=${INIT_BASE}
    VAL=$((${SCALE} * ${BASE}))
    while [ ${VAL} -le ${MAX_SIZE} -a ${TIME} -gt 0 ]
    do
        timeout ${TIME} $1 ${VAL}

        SCALE=$((${SCALE} + 1))
        if [ ${SCALE} -eq 10 ]; then
            SCALE=1
            BASE=$((${BASE} * 10))
        fi

        VAL=$((${SCALE} * ${BASE}))

        NOW=`date "+%s"`
        TIME=$((${TIMEOUT} - (${NOW} - ${START})))
    done
}

# execute for some languages
echo "# c++11"
execute ./${CPPOUT}

#-------------------------------------------------------------------------------
