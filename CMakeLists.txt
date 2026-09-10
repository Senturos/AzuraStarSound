cmake_minimum_required(VERSION 3.21)
project(AzuraStarSound VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CommonLibSSE CONFIG REQUIRED)

add_commonlibsse_plugin(${PROJECT_NAME}
    NAME "AzuraStarSound"
    AUTHOR "Senturos"
    VERSION ${PROJECT_VERSION}
    SOURCES src/plugin.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
