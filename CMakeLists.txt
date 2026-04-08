cmake_minimum_required(VERSION 3.28)
project(ST_Engine LANGUAGES CXX)

set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
set(SFML_BUILD_EXAMPLES OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME} src/steng_main.cpp
    src/steng_mainWindow.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE "./" "st_library/")
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics)
