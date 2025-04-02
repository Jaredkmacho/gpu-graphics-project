#include "gui_manager.h"
#include <iostream>

GUIManager::GUIManager(GLFWwindow* window) 
    : window(window), showDemoWindow(false), showControlsWindow(true),
      animationSpeed(1.0f), rainbowMode(true), currentShape(0) {
    backgroundColor[0] = 0.2f;
    backgroundColor[1] = 0.3f;
    backgroundColor[2] = 0.3f;
    shapeColor[0] = 1.0f;
    shapeColor[1] = 1.0f;
    shapeColor[2] = 1.0f;
}

GUIManager::~GUIManager() {
    cleanup();
}

void GUIManager::init() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void GUIManager::beginFrame() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUIManager::render(Renderer* renderer) {
    // Create the controls window
    if (showControlsWindow) {
        ImGui::Begin("Visualization Controls", &showControlsWindow);
        
        // Shape selection
        const char* shapes[] = { "Triangle", "Square", "Circle" };
        if (ImGui::Combo("Shape", &currentShape, shapes, IM_ARRAYSIZE(shapes))) {
            renderer->setShape(currentShape);
        }

        // Color controls
        ImGui::Checkbox("Rainbow Mode", &rainbowMode);
        if (!rainbowMode) {
            ImGui::ColorEdit3("Shape Color", shapeColor);
            renderer->setShapeColor(shapeColor[0], shapeColor[1], shapeColor[2]);
        }
        renderer->setRainbowMode(rainbowMode);

        // Background color
        ImGui::ColorEdit3("Background Color", backgroundColor);
        renderer->setBackgroundColor(backgroundColor[0], backgroundColor[1], backgroundColor[2]);

        // Animation speed
        ImGui::SliderFloat("Animation Speed", &animationSpeed, 0.1f, 5.0f);
        renderer->setAnimationSpeed(animationSpeed);

        // FPS controls
        static int targetFPS = 60;
        if (ImGui::SliderInt("Target FPS", &targetFPS, 0, 144)) {
            renderer->setFPSLimit(targetFPS);
        }

        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::endFrame() {
    glfwSwapBuffers(window);
}

void GUIManager::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
} 