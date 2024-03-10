#include <render/vulkan/VulkanPhysicalDevice.hpp>
#include <render/vulkan/VulkanLogicalDevice.hpp>

bool VulkanLogicalDevice::Init(VulkanPhysicalDevice& physical_device, const std::vector<const char*>& validation_layers, bool use_validation_layers) {
    VkResult vk_result;
    QueueFamilyIndices queue_family_indices = physical_device.GetQueueFamilyIndices();

    float queue_priority = 1.0f;
    VkDeviceQueueCreateInfo queue_create_info = {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.pNext = 0;
    queue_create_info.flags = 0;
    queue_create_info.queueFamilyIndex = queue_family_indices.graphics;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;

    VkPhysicalDeviceFeatures device_features = {}; // these are all bools

    uint32_t layer_count = 0;
    const char * const *layer_names = 0;
    if (use_validation_layers) {
        layer_count = validation_layers.size();
        layer_names = validation_layers.data();
    }

    VkDeviceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pNext = 0;
    create_info.flags = 0;
    create_info.queueCreateInfoCount = 1;
    create_info.pQueueCreateInfos = &queue_create_info;
    create_info.enabledLayerCount = layer_count;
    create_info.ppEnabledLayerNames = layer_names;
    create_info.enabledExtensionCount = 0;
    create_info.ppEnabledExtensionNames = 0;
    create_info.pEnabledFeatures = &device_features;

    VkDevice device;
    vk_result = vkCreateDevice(physical_device.m_Device, &create_info, 0, &device);
    if (vk_result != VK_SUCCESS) {
        printf("vkCreateDevice failed\n");
        return false;
    }

    VkQueue graphics_queue;
    vkGetDeviceQueue(device, queue_family_indices.graphics, 0, &graphics_queue);

    m_Device = device;
    m_GraphicsQueue = graphics_queue;
    return true;
}

