#!/bin/bash

#-------------------------------------------------------------------------------

# parameters
TIMEOUT=10
TRIALS=5
INIT_SCALE=1
INIT_BASE=100000
MAX_SIZE=90000000

[ $# -gt 0 ] && TIMEOUT=$1
[ $# -gt 1 ] && TRIALS=$2

# C++ 11
CPP_MAIN=main.cpp
CPP_OUT=cpp_bench
if [ ${CPP_OUT} -ot ${CPP_MAIN} ]; then
    g++ -O2 -std=c++11 ${CPP_MAIN} -o ${CPP_OUT}
fi

# Java 8
JAVA_MAIN=Main
if [ ${JAVA_MAIN}.class -ot ${JAVA_MAIN}.java ]; then
    javac ${JAVA_MAIN}.java
fi

# Python 2
PY_MAIN=main.py

# Go 1.6
GO_MAIN=go/main.go
GO_OUT=go_bench
if [ ${GO_OUT} -ot ${GO_MAIN} ]; then
    go build -o ${GO_OUT} ${GO_MAIN}
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
execute ./${CPP_OUT}
echo "# Java 8"
execute "java ${JAVA_MAIN}"
echo "# Python 2"
execute "python2 ${PY_MAIN}"
echo "# Go 1.6"
execute ./${GO_OUT}

exit 0

#-------------------------------------------------------------------------------
