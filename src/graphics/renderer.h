#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    bool init();
    bool loadShaders(const std::string& vertexPath, const std::string& fragmentPath);
    void render();
    void cleanup();
    GLFWwindow* getWindow() { return window; }  // Getter for the window
    void toggleFPSDisplay() { showFPS = !showFPS; }  // Toggle FPS display
    void setFPSLimit(int fps) { targetFPS = fps; }  // Set target FPS

private:
    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint VAO;
    GLuint VBO;
    
    std::string loadShader(const std::string& filePath);
    void checkShaderCompileErrors(GLuint shader, const std::string& type);
    void checkProgramLinkErrors(GLuint program);
    void updateFPS();  // Update FPS calculation
    void displayFPS(); // Display FPS on screen
    void limitFPS();   // Limit frame rate

    GLint colorUniformLocation;  // Add uniform location
    float time;  // Add time tracking
    int windowWidth;   // Add window dimensions
    int windowHeight;

    // FPS counter related members
    bool showFPS;
    double lastTime;
    int frameCount;
    double lastFPSUpdate;
    double currentFPS;
    int targetFPS;     // Target FPS (0 for unlimited)
    double frameTime;  // Time between frames
};