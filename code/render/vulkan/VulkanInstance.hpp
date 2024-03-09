#pragma once

#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>
#include <vulkan/vulkan_core.h>

class VulkanPhysicalDevice;

class VulkanInstance {
public:
    bool Init(SDL_Window *window);
    
private:
    VkInstance m_Instance;
    friend class VulkanPhysicalDevice;
};
