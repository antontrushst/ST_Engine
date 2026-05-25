cmake_minimum_required(VERSION 3.28)
project(Lagnia LANGUAGES CXX)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".lib")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
set(SFML_BUILD_EXAMPLES OFF)
set(SFML_USE_STATIC_STD_LIBS ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME} src/lag_main.cpp
    src/lag_mainWindow.cpp
    src/lag_screen_greet.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE "src/headers" "st_library/")
#uncomment for no-console execution on Windows
#target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics -l:libwinpthread.a)
target_compile_definitions(${PROJECT_NAME} PRIVATE SFML_STATIC)
