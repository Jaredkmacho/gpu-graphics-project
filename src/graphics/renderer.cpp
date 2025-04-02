#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <GL/glew.h>  // GLEW must be included first
#define GLUT_NO_LIB_PRAGMA  // Prevent GLUT from defining APIENTRY
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include "renderer.h"

Renderer::Renderer(int width, int height) : window(nullptr),
                                            shaderPrograms(),
                                            colorUniformLocations(),
                                            triangleVAO(0), triangleVBO(0),
                                            squareVAO(0), squareVBO(0),
                                            circleVAO(0), circleVBO(0),
                                            windowWidth(width),
                                            windowHeight(height),
                                            showFPS(false),
                                            lastTime(0.0),
                                            frameCount(0),
                                            lastFPSUpdate(0.0),
                                            currentFPS(0.0),
                                            targetFPS(0),
                                            frameTime(0.0),
                                            time(0.0f),
                                            rainbowMode(true),
                                            animationSpeed(1.0f),
                                            currentShape(0) {
    lastTime = glfwGetTime();
    backgroundColor[0] = 0.2f;
    backgroundColor[1] = 0.3f;
    backgroundColor[2] = 0.3f;
    shapeColor[0] = 1.0f;
    shapeColor[1] = 1.0f;
    shapeColor[2] = 1.0f;
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

    // Enable blending for smooth circle edges
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create shapes
    createSquare();
    createCircle();
    createTriangle();

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

bool Renderer::loadShaders(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
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

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkShaderCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Get uniform location after shader program is created and linked
    GLint colorUniformLocation = glGetUniformLocation(shaderProgram, "shapeColor");
    if (colorUniformLocation == -1) {
        std::cerr << "Failed to get uniform location for shapeColor in shader: " << name << std::endl;
        return false;
    }

    // Store the shader program and its uniform location
    shaderPrograms[name] = shaderProgram;
    colorUniformLocations[name] = colorUniformLocation;

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
    glUseProgram(shaderPrograms["default"]);
}

void Renderer::limitFPS() {
    if (targetFPS <= 0) return;  // No limit if targetFPS is 0 or negative

    double currentTime = glfwGetTime();
    frameTime = 1.0 / targetFPS;  // Calculate time between frames

    // If we're running too fast, wait
    if (currentTime - lastTime < frameTime) {
        double waitTime = frameTime - (currentTime - lastTime);
        // Wait 99% of the time waiting for events (avoids 100% CPU usage)
        glfwWaitEventsTimeout(waitTime * 0.99);
        // Wait the remaining time
        while (currentTime - lastTime < frameTime) {
            currentTime = glfwGetTime();
        }
    }

    lastTime = glfwGetTime();  // Update last frame time
}

void Renderer::createSquare() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &squareVAO);
    glGenBuffers(1, &squareVBO);
    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(squareVAO);
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Renderer::createCircle() {
    // Create a simple quad that will be used to draw the circle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);
    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Renderer::createTriangle() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);

    glBindVertexArray(triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Renderer::cleanupShapes() {
    if (squareVAO) {
        glDeleteVertexArrays(1, &squareVAO);
        squareVAO = 0;
    }
    if (squareVBO) {
        glDeleteBuffers(1, &squareVBO);
        squareVBO = 0;
    }
    if (circleVAO) {
        glDeleteVertexArrays(1, &circleVAO);
        circleVAO = 0;
    }
    if (circleVBO) {
        glDeleteBuffers(1, &circleVBO);
        circleVBO = 0;
    }
}

void Renderer::render() {
    // Get the current window size
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    
    // Set up the viewport
    glViewport(0, 0, display_w, display_h);
    
    // Clear the screen
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Update time using real time instead of frame-based time
    double currentTime = glfwGetTime();
    time = static_cast<float>(currentTime * animationSpeed);
    
    // Keep the animation continuous by using modulo
    time = fmod(time, 2.0f * 3.14159f);

    // Set color based on mode
    float red, green, blue;
    if (rainbowMode) {
        red = (sin(time) + 1.0f) / 2.0f;
        green = (sin(time + 2.0944f) + 1.0f) / 2.0f;
        blue = (sin(time + 4.1888f) + 1.0f) / 2.0f;
    } else {
        red = shapeColor[0];
        green = shapeColor[1];
        blue = shapeColor[2];
    }

    // Draw current shape
    switch (currentShape) {
        case 0:  // Triangle
            glUseProgram(shaderPrograms["default"]);
            glUniform4f(colorUniformLocations["default"], red, green, blue, 1.0f);
            glBindVertexArray(triangleVAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;
        case 1:  // Square
            glUseProgram(shaderPrograms["default"]);
            glUniform4f(colorUniformLocations["default"], red, green, blue, 1.0f);
            glBindVertexArray(squareVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            break;
        case 2:  // Circle
            glUseProgram(shaderPrograms["circle"]);
            glUniform4f(colorUniformLocations["circle"], red, green, blue, 1.0f);
            glBindVertexArray(circleVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            break;
    }
    glBindVertexArray(0);

    // Update and display FPS
    updateFPS();
    displayFPS();

    // Limit FPS if target is set
    limitFPS();
}

void Renderer::cleanup() {
    cleanupShapes();
    if (triangleVAO) {
        glDeleteVertexArrays(1, &triangleVAO);
        triangleVAO = 0;
    }
    if (triangleVBO) {
        glDeleteBuffers(1, &triangleVBO);
        triangleVBO = 0;
    }
    
    // Clean up all shader programs
    for (const auto& [name, program] : shaderPrograms) {
        if (program) {
            glDeleteProgram(program);
        }
    }
    shaderPrograms.clear();
    colorUniformLocations.clear();

    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}