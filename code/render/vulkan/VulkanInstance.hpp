#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vector>

class VulkanPhysicalDevice;

class VulkanInstance {
public:
    bool Init(SDL_Window *window, std::vector<const char *> validation_layers, bool use_validation_layers);
    
private:
    VkInstance m_Instance;
    friend class VulkanPhysicalDevice;
};
