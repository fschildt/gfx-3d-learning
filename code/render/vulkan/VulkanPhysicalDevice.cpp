#include <render/vulkan/VulkanPhysicalDevice.hpp>
#include <vulkan/vulkan_core.h>

struct QueueFamilyIndicies {
    uint32_t graphics;
};

bool VulkanPhysicalDevice::Init(VulkanInstance& instance) {
    m_Device = VK_NULL_HANDLE;
    if (!ChooseDevice(instance, &m_Device)) {
        return false;
    }

    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(m_Device, &props);
    printf("chosen device: %s\n", props.deviceName);

    return true;
}

bool VulkanPhysicalDevice::ChooseDevice(VulkanInstance& instance, VkPhysicalDevice *device) {
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
    int32_t best_score_value = -1;
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

    *device = devices[best_score_index];
    return true;
}

int VulkanPhysicalDevice::ScoreDevice(VkPhysicalDevice device) {
    int score = 0;

    std::array<VkQueueFlagBits, 1> required_queue_families = {
        VK_QUEUE_GRAPHICS_BIT
    };
    if (!SupportsQueueFamilies(device, required_queue_families)) {
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

bool VulkanPhysicalDevice::SupportsQueueFamilies(VkPhysicalDevice device, std::array<VkQueueFlagBits, 1>& flags) {
    uint32_t prop_count = 0;
    VkQueueFamilyProperties *props;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &prop_count, nullptr);
    props = (VkQueueFamilyProperties*)alloca(prop_count * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &prop_count, props);

    for (auto flag : flags) {
        bool has_flag = false;
        for (int i = 0; i < prop_count; i++) {
            if (props[i].queueFlags & flag) {
                has_flag = true;
                break;
            }
        }
        if (!has_flag) {
            return false;
        }
    }
    return true;
}

