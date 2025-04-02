#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <map>

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    bool init();
    bool loadShaders(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    void render();
    void cleanup();
    GLFWwindow* getWindow() { return window; }  // Getter for the window
    void toggleFPSDisplay() { showFPS = !showFPS; }  // Toggle FPS display
    void setFPSLimit(int fps) { targetFPS = fps; }  // Set target FPS

    // GUI control methods
    void setShape(int shape) { currentShape = shape; }
    void setRainbowMode(bool enabled) { rainbowMode = enabled; }
    void setShapeColor(float r, float g, float b) {
        shapeColor[0] = r;
        shapeColor[1] = g;
        shapeColor[2] = b;
    }
    void setBackgroundColor(float r, float g, float b) {
        backgroundColor[0] = r;
        backgroundColor[1] = g;
        backgroundColor[2] = b;
    }
    void setAnimationSpeed(float speed) { animationSpeed = speed; }

private:
    GLFWwindow* window;
    std::map<std::string, GLuint> shaderPrograms;  // Map to store multiple shader programs
    GLuint triangleVAO;
    GLuint triangleVBO;
    GLuint squareVAO;
    GLuint squareVBO;
    GLuint circleVAO;
    GLuint circleVBO;
    
    std::string loadShader(const std::string& filePath);
    void checkShaderCompileErrors(GLuint shader, const std::string& type);
    void checkProgramLinkErrors(GLuint program);
    void updateFPS();  // Update FPS calculation
    void displayFPS(); // Display FPS on screen
    void limitFPS();   // Limit frame rate
    void createSquare();
    void createCircle();
    void createTriangle();
    void cleanupShapes();

    std::map<std::string, GLint> colorUniformLocations;  // Map to store uniform locations for each shader
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

    // GUI control members
    bool rainbowMode;
    float animationSpeed;
    float backgroundColor[3];
    float shapeColor[3];
    int currentShape;
};