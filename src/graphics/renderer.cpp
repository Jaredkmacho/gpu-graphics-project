#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.h"

Renderer::Renderer(int width, int height) 
    : window(nullptr), shaderProgram(0), VAO(0), VBO(0), time(0.0f),
      windowWidth(width), windowHeight(height) {}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::init() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(windowWidth, windowHeight, "GPU Graphics Project", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    // Create and bind VAO(Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create and bind VBO(Vertex Buffer Object)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Define triangle vertices
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Copy vertex data into VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return true;
}

std::string Renderer::loadShader(const std::string& filePath) {
    std::filesystem::path path(filePath);
    if (!std::filesystem::exists(path)) {
        std::cerr << "Shader file not found: " << std::filesystem::absolute(path) << std::endl;
        return "";
    }

    std::ifstream shaderFile(path);
    std::stringstream shaderStream;

    if (shaderFile) {
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
    } else {
        std::cerr << "Could not open shader file: " << std::filesystem::absolute(path) << std::endl;
    }

    return shaderStream.str();
}

bool Renderer::loadShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    std::string vertexShaderSource = loadShader(vertexShaderPath);
    std::string fragmentShaderSource = loadShader(fragmentShaderPath);

    if (vertexShaderSource.empty() || fragmentShaderSource.empty()) {
        return false;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexShaderCode = vertexShaderSource.c_str();
    const char* fragmentShaderCode = fragmentShaderSource.c_str();

    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileErrors(vertexShader, "VERTEX");

    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileErrors(fragmentShader, "FRAGMENT");

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkShaderCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Get uniform location after shader program is created and linked
    colorUniformLocation = glGetUniformLocation(shaderProgram, "triangleColor");
    if (colorUniformLocation == -1) {
        std::cerr << "Failed to get uniform location for triangleColor" << std::endl;
        return false;
    }

    return true;
}

void Renderer::checkShaderCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[512];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Renderer::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Update time
    time += 0.01f;
    if (time > 2.0f * 3.14159f) {
        time = 0.0f;
    }

    // Create a rainbow effect using sine waves
    float red = (sin(time) + 1.0f) / 2.0f;
    float green = (sin(time + 2.0944f) + 1.0f) / 2.0f;  // Phase shift by 2π/3
    float blue = (sin(time + 4.1888f) + 1.0f) / 2.0f;   // Phase shift by 4π/3

    // Set the color uniform
    glUniform4f(colorUniformLocation, red, green, blue, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
}

void Renderer::cleanup() {
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (shaderProgram) {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}