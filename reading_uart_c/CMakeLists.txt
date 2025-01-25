cmake_minimum_required(VERSION 3.16)
project(reading_uart_c)

# Ustaw standard języka C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dodaj katalogi src/ i lib/ do ścieżek dołączania
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/lib)

# Zbierz wszystkie pliki źródłowe w katalogu src/
file(GLOB_RECURSE SOURCES
        src/*.cpp
)

# Dodanie głównego pliku wykonywalnego
add_executable(reading_uart_c ${SOURCES})
