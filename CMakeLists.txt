cmake_minimum_required(VERSION 3.24)
project(n_nmap)

set(CMAKE_CXX_STANDARD 14)

add_executable(main.cpp tcp_scanner.h work_with_ip.h)
find_package(Threads REQUIRED)