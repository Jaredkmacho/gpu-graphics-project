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

private:
    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint VAO;
    GLuint VBO;
    
    std::string loadShader(const std::string& filePath);
    void checkShaderCompileErrors(GLuint shader, const std::string& type);
    void checkProgramLinkErrors(GLuint program);

    GLint colorUniformLocation;  // Add uniform location
    float time;  // Add time tracking
    int windowWidth;   // Add window dimensions
    int windowHeight;
};