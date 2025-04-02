#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "../graphics/renderer.h"

class GUIManager {
public:
    GUIManager(GLFWwindow* window);
    ~GUIManager();

    void init();
    void beginFrame();
    void render(Renderer* renderer);
    void endFrame();
    void cleanup();
    void toggleControls() { showControlsWindow = !showControlsWindow; }

private:
    GLFWwindow* window;
    bool showDemoWindow;
    bool showControlsWindow;
    
    // GUI state
    float animationSpeed;
    float backgroundColor[3];
    float shapeColor[3];
    bool rainbowMode;
    int currentShape;
}; 