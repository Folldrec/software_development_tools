cmake_minimum_required(VERSION 4.2)

project(ComputerAlgebraProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(core
    src/MathFunction.h
    src/ComputerAlgebraInterface.h
    src/MathExpression.h
    src/Sequence.h
    src/SparseList.h
    src/SparseMatrix.h
)

enable_testing()
add_subdirectory(Tests)
