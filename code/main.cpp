#include <SDL2/SDL_video.h>
#include <render/vulkan/VulkanInstance.hpp>
#include <render/vulkan/VulkanPhysicalDevice.hpp>

int main(int argc, char **argv) {
    SDL_Window *window;
    VulkanInstance vulkan_instance;
    VulkanPhysicalDevice vulkan_physical_device;

    window = SDL_CreateWindow("game1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 720, SDL_WINDOW_VULKAN);
    if (!window) {
        return 0;
    }

    if (!vulkan_instance.Init(window)) {
        return 0;
    }

    if (!vulkan_physical_device.Init(vulkan_instance)) {
        return 0;
    }

    return 0;
}
