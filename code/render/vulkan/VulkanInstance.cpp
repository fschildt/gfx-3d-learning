#include <render/vulkan/VulkanInstance.hpp>
#include <vulkan/vulkan_core.h>
#include <SDL2/SDL_vulkan.h>
#include <basic/defs.hpp>

#include <stdio.h>
#include <vector>

bool VulkanInstance::Init(SDL_Window *window) {
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
    if (!SDL_Vulkan_GetInstanceExtensions(window, &ext_count, 0)) {
        return false;
    }
    ext_names = (const char **)alloca(ext_count * sizeof(char*));
    if (!SDL_Vulkan_GetInstanceExtensions(window, &ext_count, ext_names)) {
        return false;
    }
    for (int i = 0; i < ext_count; i++) {
        printf("ext_name = %s\n", ext_names[i]);
    }

#if NDEBUG
    uint32_t layer_count = 0;
    const char **layer_names = 0;
#else
    const char *layer_names[] = {"VK_LAYER_KHRONOS_validation"};
    uint32_t layer_count = ARRAY_COUNT(layer_names);
#endif

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

