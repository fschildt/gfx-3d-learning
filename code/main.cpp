#include <render/vulkan/VulkanLogicalDevice.hpp>
#include <SDL2/SDL_video.h>
#include <render/vulkan/VulkanInstance.hpp>
#include <render/vulkan/VulkanPhysicalDevice.hpp>

#include <vector>

int main(int argc, char **argv) {
    SDL_Window *window;

    window = SDL_CreateWindow("game1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 720, SDL_WINDOW_VULKAN);
    if (!window) {
        return 0;
    }

    bool use_validation_layers = true;
    const std::vector<const char*> validation_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

    VulkanInstance vulkan_instance;
    VulkanPhysicalDevice vulkan_physical_device;
    QueueFamilyIndices queue_family_indices;
    VulkanLogicalDevice vulkan_logical_device;

    if (!vulkan_instance.Init(window, validation_layers, use_validation_layers)) {
        return 0;
    }

    if (!vulkan_physical_device.Init(vulkan_instance)) {
        return 0;
    }

    if (!vulkan_logical_device.Init(vulkan_physical_device, validation_layers, use_validation_layers)) {
        return 0;
    }

    return 0;
}
