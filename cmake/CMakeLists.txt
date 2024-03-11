cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_COMPILER clang++)

# set binary directory
set(APP_ROOT_DIR  "${CMAKE_CURRENT_LIST_DIR}/..")
set(APP_CODE_DIR  "${APP_ROOT_DIR}/code")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)


project(gfx-3d-learning)

add_executable(gfx-3d-learning)
target_sources(gfx-3d-learning PUBLIC
    ${APP_CODE_DIR}/main.cpp
    ${APP_CODE_DIR}/VulkanRenderer.cpp
)
target_include_directories(gfx-3d-learning PUBLIC
    ${APP_CODE_DIR}
)
target_link_libraries(gfx-3d-learning SDL2 vulkan)
