cmake_minimum_required(VERSION 2.8)
project(test)

set(ROOT_PATH ${PROJECT_SOURCE_DIR}/../..)
set(MINIGLOG_LIBS ${ROOT_PATH}/build/lib/libminiglog.so)
set(MINIGLOG_INCLUDE_DIRS ${ROOT_PATH}/build/include)

file(GLOB test_dep
    "*.h"
    "*.cpp"
)

include_directories( ${MINIGLOG_INCLUDE_DIRS})
add_executable( test ${test_dep} )
target_link_libraries( test ${MINIGLOG_LIBS} )
