cmake_minimum_required(VERSION 3.11)
project(RICE8)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
Set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2
    GIT_TAG        develop
    GIT_PROGRESS
)

FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
    GIT_SHALLOW ON
    GIT_PROGRESS
)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master
)

FetchContent_MakeAvailable(imgui)
FetchContent_MakeAvailable(sol2)
FetchContent_MakeAvailable(raylib)

file(GLOB SRC_FILES
    source/*.cpp
    source/*.h
    source/gui/*.cpp
    source/gui/*.h
    source/cartridge/*.cpp
    source/cartridge/*.h
    source/cartridge/lua/*.cpp
    source/cartridge/lua/*.h
    source/cartridge/sprite/*.cpp
    source/cartridge/sprite/*.h
)

list(APPEND SRC_FILES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    third_party/rlimgui/rlImGui.cpp
    third_party/imgui_texteditor/TextEditor.cpp
    source/gui/pixeleditor.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

find_package(Lua REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${sol2_SOURCE_DIR}/single
    ${LUA_INCLUDE_DIR}
    ${imgui_SOURCE_DIR}
    third_party/rlimgui
    third_party/imgui_texteditor
    ${CMAKE_CURRENT_SOURCE_DIR}/source/cartridge
    ${CMAKE_CURRENT_SOURCE_DIR}/source/cartridge/lua
    ${CMAKE_CURRENT_SOURCE_DIR}/source/cartridge/sprite

)
target_link_libraries(${PROJECT_NAME} PRIVATE sol2 raylib ${LUA_LIBRARIES})
