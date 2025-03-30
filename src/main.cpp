#include <iostream>
#include <string>
#include "graphics\renderer.h"
#include "gpu\gpu_utils.h"

int main() {
    // Initialize the graphics context
    Renderer renderer(800, 600);
    if (!renderer.init()) {
        std::cerr << "Failed to initialize the renderer." << std::endl;
        return -1;
    }

    // Load shaders
    if (!renderer.loadShaders("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl")) {
        std::cerr << "Failed to load shaders." << std::endl;
        return -1;
    }

    // Main loop for rendering
    while (!glfwWindowShouldClose(renderer.getWindow())) {
        renderer.render();
        glfwPollEvents();  // Process window events
    }

    return 0;
}