#include <SDL2/SDL_video.h>
#include <SDL2/SDL_vulkan.h>
#include <VulkanRenderer.hpp>

#include <vector>
#include <vulkan/vulkan_core.h>

int main(int argc, char **argv) {
    SDL_Window *window;

    window = SDL_CreateWindow("game1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 720, SDL_WINDOW_VULKAN);
    if (!window) {
        return 0;
    }

    VulkanRenderer renderer;
    if (!renderer.Init(window, true)) {
        return 0;
    }


    return 0;
}
