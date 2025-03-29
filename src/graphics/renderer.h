#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init();
    bool loadShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    void render();
    void cleanup();

private:
    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint VAO;
    GLuint VBO;
    
    std::string loadShader(const std::string& filePath);
    void checkShaderCompileErrors(GLuint shader, const std::string& type);
};