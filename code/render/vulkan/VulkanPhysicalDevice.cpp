#include <render/vulkan/VulkanPhysicalDevice.hpp>
#include <vulkan/vulkan_core.h>

bool VulkanPhysicalDevice::Init(VulkanInstance& instance) {
    m_Device = VK_NULL_HANDLE;
    if (!PickDevice(instance, m_Device)) {
        return false;
    }

    return true;
}

QueueFamilyIndices VulkanPhysicalDevice::GetQueueFamilyIndices() {
    QueueFamilyIndices queue_family_indices;

    uint32_t prop_count = 0;
    VkQueueFamilyProperties *props;
    vkGetPhysicalDeviceQueueFamilyProperties(m_Device, &prop_count, nullptr);
    props = (VkQueueFamilyProperties*)alloca(prop_count * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(m_Device, &prop_count, props);

    for (int i = 0; i < prop_count; i++) {
        if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queue_family_indices.graphics = i;
        }
    }

    return queue_family_indices;
}

bool VulkanPhysicalDevice::PickDevice(VulkanInstance& instance, VkPhysicalDevice& device) {
    VkResult vk_result;

    uint32_t device_count;
    VkPhysicalDevice *devices;
    vk_result = vkEnumeratePhysicalDevices(instance.m_Instance, &device_count, nullptr);
    if (vk_result != VK_SUCCESS) {
        return false;
    }
    devices = (VkPhysicalDevice*)alloca(device_count * sizeof(VkPhysicalDevice));
    vk_result = vkEnumeratePhysicalDevices(instance.m_Instance, &device_count, devices);
    if (vk_result != VK_SUCCESS) {
        return false;
    }

    int32_t best_score_index = -1;
    int32_t best_score_value = 0;
    for (int i = 0; i < device_count; i++) {
        int score = ScoreDevice(devices[i]);
        if (score > best_score_value) {
            best_score_index = i;
            best_score_value = score;
        }
    }
    if (best_score_index < 0) {
        return false;
    }

    device = devices[best_score_index];
    return true;
}

int VulkanPhysicalDevice::ScoreDevice(VkPhysicalDevice device) {
    int score = 0;

    if (!SupportsQueueFamilies(device)) {
        return 0;
    }

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);
    // Discrete GPUs have a significant performance advantage
    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    // Maximum possible size of textures affects graphics quality
    score += properties.limits.maxImageDimension2D;

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);
    // Application can't function without geometry shaders
    if (!features.geometryShader) {
        return 0;
    }

    return score;
}

bool VulkanPhysicalDevice::SupportsQueueFamilies(VkPhysicalDevice device) {
    uint32_t prop_count = 0;
    VkQueueFamilyProperties *props;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &prop_count, nullptr);
    props = (VkQueueFamilyProperties*)alloca(prop_count * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &prop_count, props);

    bool has_graphics = false;
    for (int i = 0; i < prop_count; i++) {
        if (props->queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            return true;
        }
    }
    return false;
}

