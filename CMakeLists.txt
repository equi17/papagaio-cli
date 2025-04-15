cmake_minimum_required(VERSION 3.10)
project(papagaio-cli LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(extern/SQLiteCpp)

set(SOURCES
    src/main.cpp
    src/deck.cpp
    src/cardmanager.cpp
    src/card.cpp
)

include_directories(include)

add_executable(papagaio-cli ${SOURCES})

target_link_libraries(papagaio-cli PRIVATE 
    SQLiteCpp
)