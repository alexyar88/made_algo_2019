cmake_minimum_required(VERSION 3.15)
project(15)

set(CMAKE_CXX_STANDARD 17)

add_executable(15 main.cpp dsu.cpp dsu.h graph.h graph.cpp min_path_find.cpp min_path_find.h point_utils.cpp point_utils.h kruskal_approximation.cpp kruskal_approximation.h experiment.cpp experiment.h)