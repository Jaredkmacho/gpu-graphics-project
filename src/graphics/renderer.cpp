#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <GL/glew.h>  // GLEW must be included first
#define GLUT_NO_LIB_PRAGMA  // Prevent GLUT from defining APIENTRY
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include "renderer.h"

Renderer::Renderer(int width, int height) 
    : window(nullptr), shaderProgram(0), VAO(0), VBO(0), time(0.0f),
      windowWidth(width), windowHeight(height),
      showFPS(false), lastTime(0.0), frameCount(0),
      lastFPSUpdate(0.0), currentFPS(0.0),
      targetFPS(0), frameTime(0.0) {  // Initialize FPS limit members
    lastTime = glfwGetTime();
}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::init() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Initialize GLUT
    int argc = 1;
    char* argv[1] = { (char*)"Something" };
    glutInit(&argc, argv);

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

void Renderer::updateFPS() {
    double currentTime = glfwGetTime();
    frameCount++;

    // Update FPS every second
    if (currentTime - lastFPSUpdate >= 1.0) {
        currentFPS = frameCount / (currentTime - lastFPSUpdate);
        frameCount = 0;
        lastFPSUpdate = currentTime;
    }
}

void Renderer::displayFPS() {
    if (!showFPS) return;

    // Set up text rendering (using GLFW's built-in text rendering)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable shader program for text rendering
    glUseProgram(0);

    // Set text color (white)
    glColor3f(1.0f, 1.0f, 1.0f);

    // Position the text
    glRasterPos2i(10, 20);

    // Create FPS string
    std::string fpsText = "FPS: " + std::to_string(static_cast<int>(currentFPS));

    // Draw the text
    for (char c : fpsText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Restore matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Re-enable shader program
    glUseProgram(shaderProgram);
}

void Renderer::limitFPS() {
    if (targetFPS <= 0) return;  // No limit if targetFPS is 0 or negative

    double currentTime = glfwGetTime();
    frameTime = 1.0 / targetFPS;  // Calculate time between frames

    // If we're running too fast, wait
    if (currentTime - lastTime < frameTime) {
        double waitTime = frameTime - (currentTime - lastTime);
        glfwWaitEventsTimeout(waitTime);
    }

    lastTime = glfwGetTime();  // Update last frame time
}

void Renderer::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Update time using real time instead of frame-based time
    double currentTime = glfwGetTime();
    time = static_cast<float>(currentTime * 1);  // Slower animation speed (0.5 cycles per second)
    
    // Keep the animation continuous by using modulo
    time = fmod(time, 2.0f * 3.14159f);

    // Create a rainbow effect using sine waves
    float red = (sin(time) + 1.0f) / 2.0f;
    float green = (sin(time + 2.0944f) + 1.0f) / 2.0f;  // Phase shift by 2π/3
    float blue = (sin(time + 4.1888f) + 1.0f) / 2.0f;   // Phase shift by 4π/3

    // Set the color uniform
    glUniform4f(colorUniformLocation, red, green, blue, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Update and display FPS
    updateFPS();
    displayFPS();

    // Limit FPS if target is set
    limitFPS();

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