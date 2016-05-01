#!/bin/bash

#-------------------------------------------------------------------------------

# parameters
TIMEOUT=10
TRIALS=5
INIT_SCALE=1
INIT_BASE=1000000
MAX_SIZE=90000000

# C++ 11
CPP_MAIN=main.cpp
CPP_OUT=a.out
if [ ${CPP_OUT} -ot ${CPP_MAIN} ]; then
    g++ -O3 -std=c++11 ${CPP_MAIN} -o ${CPP_OUT}
fi

# Java 8
JAVA_MAIN=Main
if [ ${JAVA_MAIN}.class -ot ${JAVA_MAIN}.java ]; then
    javac ${JAVA_MAIN}.java
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
        timeout ${TIME} $1 ${VAL} ${TRIALS}

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
echo "# C++ 11"
execute "./${CPP_OUT}"
echo "# Java 8"
execute "java ${JAVA_MAIN}"

#-------------------------------------------------------------------------------
