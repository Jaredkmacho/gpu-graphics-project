#include <iostream>
#include <string>
#include "graphics\renderer.h"
#include "gpu\gpu_utils.h"
#include "gui\gui_manager.h"

// Structure to hold both renderer and GUI pointers
struct WindowData {
    Renderer* renderer;
    GUIManager* gui;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        data->renderer->toggleFPSDisplay();
    }
    else if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        data->gui->toggleControls();
    }
}

int main() {
    std::cout << "Starting application..." << std::endl;
    
    // Initialize the graphics context
    Renderer renderer(800, 600);
    if (!renderer.init()) {
        std::cerr << "Failed to initialize the renderer." << std::endl;
        return -1;
    }
    std::cout << "Renderer initialized successfully" << std::endl;

    // Initialize GUI
    GUIManager gui(renderer.getWindow());
    gui.init();
    std::cout << "GUI initialized successfully" << std::endl;

    // Set up key callback
    GLFWwindow* window = renderer.getWindow();
    WindowData* windowData = new WindowData{&renderer, &gui};
    glfwSetWindowUserPointer(window, windowData);
    glfwSetKeyCallback(window, key_callback);
    std::cout << "Key callback set up" << std::endl;

    // Load shaders
    if (!renderer.loadShaders("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl")) {
        std::cerr << "Failed to load shaders." << std::endl;
        return -1;
    }
    std::cout << "Shaders loaded successfully" << std::endl;

    // Set FPS limit to 60
    renderer.setFPSLimit(60);
    std::cout << "FPS limit set to 60" << std::endl;

    std::cout << "Entering main loop..." << std::endl;
    // Main loop for rendering
    while (!glfwWindowShouldClose(window)) {
        // Process input
        glfwPollEvents();

        // Start the Dear ImGui frame
        gui.beginFrame();
        
        // Render the OpenGL content
        renderer.render();
        
        // Render the GUI
        gui.render(&renderer);
        
        // End frame and swap buffers
        gui.endFrame();
    }
    std::cout << "Main loop ended" << std::endl;

    return 0;
}