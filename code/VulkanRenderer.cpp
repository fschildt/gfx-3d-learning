#include <SDL2/SDL_vulkan.h>
#include <VulkanRenderer.hpp>
#include <unordered_set>

bool VulkanRenderer::Init(SDL_Window *window, bool debug_output) {
    m_Window = window;

    bool use_validation_layers = debug_output;
    std::vector<const char*> validation_layers;
    if (use_validation_layers) {
        validation_layers.push_back("VK_LAYER_KHRONOS_validation");
    }

    if (!CreateInstance(validation_layers, use_validation_layers)) {
        return false;
    }

    if (!CreateSurface()) {
        return false;
    }

    if (!SelectPhysicalDevice()) {
        return false;
    }

    if (!CreateLogicalDevice(validation_layers, use_validation_layers)) {
        return false;
    }

    return true;
}

bool VulkanRenderer::CreateInstance(std::vector<const char*>& validation_layers, bool use_validation_layers) {
    VkResult vk_result;

    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = 0;
    app_info.pApplicationName = "application name";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_3;

    uint32_t ext_count;
    const char **ext_names;
    if (!SDL_Vulkan_GetInstanceExtensions(m_Window, &ext_count, 0)) {
        return false;
    }
    ext_names = (const char **)alloca(ext_count * sizeof(char*));
    if (!SDL_Vulkan_GetInstanceExtensions(m_Window, &ext_count, ext_names)) {
        return false;
    }

    uint32_t layer_count = 0;
    const char **layer_names = 0;
    if (use_validation_layers) {
        layer_count = validation_layers.size();
        layer_names = validation_layers.data();
    }

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pNext = 0;
    create_info.flags = 0;
    create_info.pApplicationInfo = &app_info;
    create_info.enabledLayerCount = layer_count;
    create_info.ppEnabledLayerNames = layer_names;
    create_info.enabledExtensionCount = ext_count;
    create_info.ppEnabledExtensionNames = ext_names;

    vk_result = vkCreateInstance(&create_info, nullptr, &m_Instance);
    if (vk_result != VK_SUCCESS) {
        printf("vkCreateInstance failed %d\n", vk_result);
        return false;
    }

    return true;
}

bool VulkanRenderer::CreateSurface() {
    if (!SDL_Vulkan_CreateSurface(m_Window, m_Instance, &m_Surface)) {
        return false;
    }
    return true;
}


bool VulkanRenderer::SelectPhysicalDevice() {
    VkResult vk_result;

    uint32_t device_count;
    VkPhysicalDevice *devices;
    vk_result = vkEnumeratePhysicalDevices(m_Instance, &device_count, nullptr);
    if (vk_result != VK_SUCCESS) {
        return false;
    }
    devices = (VkPhysicalDevice*)alloca(device_count * sizeof(VkPhysicalDevice));
    vk_result = vkEnumeratePhysicalDevices(m_Instance, &device_count, devices);
    if (vk_result != VK_SUCCESS) {
        return false;
    }

    int32_t best_score_index = -1;
    int32_t best_score_value = 0;
    for (int i = 0; i < device_count; i++) {
        int score = ScorePhysicalDevice(devices[i]);
        if (score > best_score_value) {
            best_score_index = i;
            best_score_value = score;
        }
    }
    if (best_score_index < 0) {
        return false;
    }

    m_PhysicalDevice = devices[best_score_index];
    return true;
}


int VulkanRenderer::ScorePhysicalDevice(VkPhysicalDevice physical_device) {
    int score = 0;

    QueueFamilyIndices queue_family_indices = GetQueueFamilyIndices(physical_device);
    if (!queue_family_indices.present.has_value() ||
        !queue_family_indices.graphics.has_value()) {
        return 0;
    }

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physical_device, &properties);
    // Discrete GPUs have a significant performance advantage
    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    // Maximum possible size of textures affects graphics quality
    score += properties.limits.maxImageDimension2D;

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(physical_device, &features);
    // Application can't function without geometry shaders
    if (!features.geometryShader) {
        return 0;
    }

    return score;
}

QueueFamilyIndices VulkanRenderer::GetQueueFamilyIndices(VkPhysicalDevice physical_device) {
    QueueFamilyIndices queue_family_indices;

    uint32_t queue_family_count = 0;
    VkQueueFamilyProperties *queue_family_properties;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
    queue_family_properties = (VkQueueFamilyProperties*)alloca(queue_family_count * sizeof(VkQueueFamilyProperties));

    for (int i = 0; i < queue_family_count; i++) {
        if (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queue_family_indices.graphics = i;
        }

        VkBool32 has_present;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, m_Surface, &has_present);
        if (has_present) {
            queue_family_indices.present = i;
        }
    }

    return queue_family_indices;
}

bool VulkanRenderer::CreateLogicalDevice(const std::vector<const char*>& validation_layers, bool use_validation_layers) {
    VkResult vk_result;


    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

    QueueFamilyIndices queue_family_indices = GetQueueFamilyIndices(m_PhysicalDevice);
    std::unordered_set<uint32_t> unique_queue_family_indices = {queue_family_indices.graphics.value(), queue_family_indices.present.value()};
    float queuePriority = 1.0f;
    for (uint32_t queue_family_index : unique_queue_family_indices) {
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.pNext = 0;
        queue_create_info.flags = 0;
        queue_create_info.queueFamilyIndex = queue_family_index;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queuePriority;
        queue_create_infos.push_back(queue_create_info);
    }


    uint32_t layer_count = 0;
    const char * const *layer_names = 0;
    if (use_validation_layers) {
        layer_count = validation_layers.size();
        layer_names = validation_layers.data();
    }


    VkPhysicalDeviceFeatures device_features = {};


    VkDeviceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pNext = 0;
    create_info.flags = 0;
    create_info.queueCreateInfoCount = queue_create_infos.size();
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.enabledLayerCount = layer_count;
    create_info.ppEnabledLayerNames = layer_names;
    create_info.enabledExtensionCount = 0;
    create_info.ppEnabledExtensionNames = 0;
    create_info.pEnabledFeatures = &device_features;


    VkDevice logical_device;
    vk_result = vkCreateDevice(m_PhysicalDevice, &create_info, 0, &logical_device);
    if (vk_result != VK_SUCCESS) {
        printf("vkCreateDevice failed\n");
        return false;
    }

    vkGetDeviceQueue(logical_device, queue_family_indices.graphics.value(), 0, &m_GraphicsQueue);
    vkGetDeviceQueue(logical_device, queue_family_indices.present.value(), 0, &m_PresentQueue);


    m_LogicalDevice = logical_device;
    return true;
}


