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

set(IMGUI_VERSION 1.91.5)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
    imgui
    URL "https://github.com/ocornut/imgui/archive/v${IMGUI_VERSION}.zip")
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG v3.0)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" ON)
FetchContent_MakeAvailable(imgui-sfml)

add_executable(${PROJECT_NAME} src/lag_main.cpp
    src/lag_mainWindow.cpp
    src/lag_screen_greet.cpp
    src/lag_screen_scene.cpp
    src/lag_inGame_display.cpp
    src/lag_grid.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE "src/headers" "st_library/")
#uncomment for no-console execution on Windows
#target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        SFML::Graphics
        ImGui-SFML::ImGui-SFML
        -l:libwinpthread.a
        ole32
        uuid)
target_compile_definitions(${PROJECT_NAME} PRIVATE SFML_STATIC)
