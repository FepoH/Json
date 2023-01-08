cmake_minimum_required(VERSION 3.0)

project(Json)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -Wno-sign-compare -Wno-unused-but-set-variable -Wno-unused-variable -Wno-maybe-uninitialized -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -Wno-sign-compare -Wno-unused-but-set-variable -Wno-unused-variable -Wno-maybe-uninitialized -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")


set(SRC
    json/json.cpp
    json/json_parser.cpp

)

add_library(fepoh_json SHARED ${SRC})

set(
    LIB
    fepoh_json

)

add_executable(test json/test.cpp ${SRC})
add_dependencies(test fepoh_json)
target_link_libraries(test ${LIB})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)