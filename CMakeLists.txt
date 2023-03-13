cmake_minimum_required(VERSION 3.24)
project(n_nmap)

set(CMAKE_CXX_STANDARD 14)

add_executable(n_nmap main.cpp TCPScanner.h)
