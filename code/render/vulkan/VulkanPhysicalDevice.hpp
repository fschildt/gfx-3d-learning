#pragma once

#include <vulkan/vulkan.h>
#include <render/vulkan/VulkanInstance.hpp>
#include <render/vulkan/VulkanLogicalDevice.hpp>


struct QueueFamilyIndices {
    uint32_t graphics;
};


class VulkanPhysicalDevice {
public:
    bool Init(VulkanInstance& instance);
    QueueFamilyIndices GetQueueFamilyIndices();

private:
    bool PickDevice(VulkanInstance& instance, VkPhysicalDevice& device);
    int ScoreDevice(VkPhysicalDevice device);
    bool SupportsQueueFamilies(VkPhysicalDevice device);

private:
    VkPhysicalDevice m_Device;
    VkQueue graphicsQueue;

    friend class VulkanLogicalDevice;
};

