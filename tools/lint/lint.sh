#!/bin/sh
ABSOLUTE_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

LINT="cpplint.py"

JNI_DIR="${ABSOLUTE_PATH}/../../jni/*"
echo $JNI_DIR

python cpplint.py $( find ${JNI_DIR} -name \*.h -or -name \*.cpp)
