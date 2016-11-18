#!/bin/sh
ABSOLUTE_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

CLANG_FORMAT="clang-format"

JNI_DIR="${ABSOLUTE_PATH}/../../jni"
echo $JNI_DIR
for dir in $JNI_DIR
do
    echo "Formatting code under $dir/"
    find "$dir" \( -name '*.h' -or -name '*.cpp' -or -name '*.hpp' \) -print0 | xargs -0 "$CLANG_FORMAT" -i
done
