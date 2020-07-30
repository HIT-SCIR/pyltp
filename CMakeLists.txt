cmake_minimum_required(VERSION 2.8.12)
project(pyltp)

add_subdirectory(ltp)
add_subdirectory(pybind11)
pybind11_add_module(pyltp SHARED src/pyltp.cpp)
target_link_libraries (pyltp PRIVATE
  ner_static_lib
  parser_static_lib
  postagger_static_lib
  segmentor_static_lib
  splitsnt_static_lib
  srl_static_lib
)
target_include_directories(pyltp PRIVATE
    ltp/include
)
