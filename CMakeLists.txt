cmake_minimum_required(VERSION 2.8.12)
project(pyltp)

set(PYBIND11_CPP_STANDARD -std=c++14)
add_subdirectory(pybind11)
add_subdirectory(ltp)

add_library(pyltp MODULE src/pyltp.cpp)
target_link_libraries(
        pyltp PRIVATE
        pybind11::module
        ner_static_lib
        parser_static_lib
        postagger_static_lib
        segmentor_static_lib
        splitsnt_static_lib
        srl_static_lib
)
target_include_directories(
        pyltp PRIVATE
        ltp/include
)
set_target_properties(
        pyltp
        PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
        SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
