# GPU Graphics Project

## Overview
The GPU Graphics Project is a modern C++ graphics application that leverages OpenGL for high-performance graphics rendering on Windows. This project serves as a foundation for creating graphics applications that utilize modern GPU features for rendering high-quality visuals.

## Demo
Here's a demonstration of the project in action:

https://github.com/Jaredkmacho/gpu-graphics-project/raw/main/docs/video/rainbow-triangle.mp4

## Features
- Modern OpenGL rendering pipeline
- Shader-based graphics processing
- Efficient GPU memory management
- CMake-based build system
- Comprehensive error handling and logging

## Dependencies
- Windows 10 or later
- Visual Studio 2019 or later
- C++17 or later
- CMake 3.15 or later
- OpenGL 4.5 or later
- GLFW 3.3 or later
- GLEW 2.2 or later
- GLM (OpenGL Mathematics)
- vcpkg for dependency management

## Project Structure
```
gpu-graphics-project
├── src/
│   ├── main.cpp              # Application entry point
│   ├── graphics/             # Graphics-related source files
│   │   ├── renderer.cpp      # Renderer implementation
│   │   └── renderer.h        # Renderer header
│   └── gpu/                  # GPU utilities
│       ├── gpu_utils.cpp     # GPU utility functions
│       └── gpu_utils.h       # GPU utility headers
├── include/                  # Public headers
│   ├── renderer.h           # Public renderer interface
│   └── gpu_utils.h          # Public GPU utilities
├── shaders/                  # GLSL shader files
│   ├── vertex_shader.glsl   # Vertex shader
│   └── fragment_shader.glsl # Fragment shader
├── docs/                     # Documentation
│   └── api_reference.md     # API documentation
├── tests/                    # Test files
├── CMakeLists.txt           # Main CMake configuration
├── .gitignore               # Git ignore rules
└── README.md                # This file
```

## Building from Source

### Prerequisites
1. Install Visual Studio 2019 or later with C++ development tools
2. Install CMake 3.15 or later
3. Install vcpkg and set up the VCPKG_ROOT environment variable
4. Install required dependencies via vcpkg:
   ```bash
   vcpkg install glfw3:x64-windows
   vcpkg install glew:x64-windows
   vcpkg install glm:x64-windows
   ```

### Build Instructions
1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd gpu-graphics-project
   ```

2. **Create and enter build directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake**:
   ```bash
   cmake .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
   ```

4. **Build the project**:
   ```bash
   cmake --build .
   ```

## Usage
After building, the executable will be available in your build directory:
```bash
# From build directory
Debug\GPUGraphicsProject.exe
```

## Development
### Code Style
- Follow the project's coding standards
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and modular

### Adding New Features
1. Create a new branch for your feature
2. Implement your changes
3. Update documentation
4. Submit a pull request

## Contributing
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
- OpenGL community for documentation and resources
- GLFW and GLEW maintainers
- Contributors to this project