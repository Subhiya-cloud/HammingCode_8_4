cmake_minimum_required(VERSION 3.10)

# Название проекта
project(HammingCode)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_library(Hamming hamming.cpp)


# Исполняемый файл
add_executable(hamming main.cpp hamming.cpp)
