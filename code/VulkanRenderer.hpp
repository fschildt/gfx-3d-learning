#pragma once

#include <vulkan/vulkan_core.h>
#include <SDL2/SDL_vulkan.h>

#include <vector>
#include <optional>


struct QueueFamilyIndices {
    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;
};


class VulkanRenderer {
public:
    bool Init(SDL_Window *window, bool debug_output);

private:
    bool CreateInstance(std::vector<const char*>& validation_layers, bool use_validation_layers);
    bool CreateSurface();

    bool SelectPhysicalDevice();
    int ScorePhysicalDevice(VkPhysicalDevice device);
    QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice device);

    bool CreateLogicalDevice(const std::vector<const char*>& validation_layers, bool use_validation_layers);


private:
    SDL_Window *m_Window;
    VkInstance m_Instance;
    VkSurfaceKHR m_Surface;
    VkPhysicalDevice m_PhysicalDevice;
    VkDevice m_LogicalDevice;
    VkQueue m_GraphicsQueue;
    VkQueue m_PresentQueue;
};

