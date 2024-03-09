cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_COMPILER clang++)

# set binary directory
set(GAME1_ROOT_DIR  "${CMAKE_CURRENT_LIST_DIR}/..")
set(GAME1_CODE_DIR  "${GAME1_ROOT_DIR}/code")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)


project(game1_project)

add_executable(game1)
target_sources(game1 PUBLIC
    ${GAME1_CODE_DIR}/main.cpp
    ${GAME1_CODE_DIR}/render/vulkan/VulkanInstance.cpp
    ${GAME1_CODE_DIR}/render/vulkan/VulkanPhysicalDevice.cpp
)
target_include_directories(game1 PUBLIC
    ${GAME1_CODE_DIR}
)
target_link_libraries(game1 SDL2 vulkan)
