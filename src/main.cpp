#include <iostream>
#include <string>
#include "graphics\renderer.h"
#include "gpu\gpu_utils.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
        renderer->toggleFPSDisplay();
    }
}

int main() {
    // Initialize the graphics context
    Renderer renderer(800, 600);
    if (!renderer.init()) {
        std::cerr << "Failed to initialize the renderer." << std::endl;
        return -1;
    }

    // Set up key callback
    GLFWwindow* window = renderer.getWindow();
    glfwSetWindowUserPointer(window, &renderer);
    glfwSetKeyCallback(window, key_callback);

    // Load shaders
    if (!renderer.loadShaders("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl")) {
        std::cerr << "Failed to load shaders." << std::endl;
        return -1;
    }

    // Set FPS limit to 60
    renderer.setFPSLimit(60);

    // Main loop for rendering
    while (!glfwWindowShouldClose(window)) {
        renderer.render();
        glfwPollEvents();  // Process window events
    }

    return 0;
}