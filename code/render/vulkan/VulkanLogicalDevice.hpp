#pragma once

#include <render/vulkan/VulkanPhysicalDevice.hpp>

class VulkanLogicalDevice {
public:
    bool Init(VulkanPhysicalDevice& physical_device, const std::vector<const char*>& validation_layers, bool use_validation_layers);

private:
    VkDevice m_Device;
    VkQueue m_GraphicsQueue;
};
