#pragma once

#include <vulkan/vulkan.h>
#include <render/vulkan/VulkanInstance.hpp>
#include <array>

class VulkanPhysicalDevice {
public:
    bool Init(VulkanInstance& instance);

private:
    bool ChooseDevice(VulkanInstance& instance, VkPhysicalDevice *device);
    int ScoreDevice(VkPhysicalDevice device);
    bool SupportsQueueFamilies(VkPhysicalDevice device, std::array<VkQueueFlagBits, 1>& flags);

private:
    VkPhysicalDevice m_Device;
};

