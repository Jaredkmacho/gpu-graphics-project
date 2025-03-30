# GPU Graphics Project API Reference

## OpenGL/GLEW/GLFW Functions

### Window and Context Management

#### `glfwInit()`
- **Description**: Initializes GLFW library
- **Returns**: `GL_TRUE` on success, `GL_FALSE` on failure
- **Usage**: Must be called before any other GLFW functions

#### `glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)`
- **Description**: Creates a window and its associated OpenGL context
- **Parameters**:
  - `width`: Window width in pixels
  - `height`: Window height in pixels
  - `title`: Window title
  - `monitor`: Monitor for fullscreen mode (NULL for windowed)
  - `share`: Window to share resources with (NULL for none)
- **Returns**: `GLFWwindow*` pointer or NULL on failure

#### `glfwMakeContextCurrent(GLFWwindow* window)`
- **Description**: Makes the window's context current for the calling thread
- **Parameters**:
  - `window`: Window whose context to make current
- **Returns**: void

#### `glfwTerminate()`
- **Description**: Terminates GLFW library and frees resources
- **Returns**: void

### Buffer Management

#### `glGenVertexArrays(GLsizei n, GLuint* arrays)`
- **Description**: Generates vertex array object names
- **Parameters**:
  - `n`: Number of VAO names to generate
  - `arrays`: Array to store generated names
- **Returns**: void

#### `glGenBuffers(GLsizei n, GLuint* buffers)`
- **Description**: Generates buffer object names
- **Parameters**:
  - `n`: Number of buffer names to generate
  - `buffers`: Array to store generated names
- **Returns**: void

#### `glBindVertexArray(GLuint array)`
- **Description**: Binds a vertex array object
- **Parameters**:
  - `array`: VAO name to bind
- **Returns**: void

#### `glBindBuffer(GLenum target, GLuint buffer)`
- **Description**: Binds a named buffer object
- **Parameters**:
  - `target`: Target to bind to (e.g., GL_ARRAY_BUFFER)
  - `buffer`: Buffer name to bind
- **Returns**: void

#### `glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)`
- **Description**: Creates and initializes a buffer object's data store
- **Parameters**:
  - `target`: Target buffer type
  - `size`: Size of data in bytes
  - `data`: Pointer to data
  - `usage`: Expected usage pattern (e.g., GL_STATIC_DRAW)
- **Returns**: void

### Shader Management

#### `glCreateShader(GLenum shaderType)`
- **Description**: Creates a shader object
- **Parameters**:
  - `shaderType`: Type of shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER)
- **Returns**: `GLuint` shader ID or 0 on failure

#### `glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length)`
- **Description**: Sets the source code in a shader object
- **Parameters**:
  - `shader`: Shader object
  - `count`: Number of strings
  - `string`: Array of source code strings
  - `length`: Array of string lengths (NULL for null-terminated)
- **Returns**: void

#### `glCompileShader(GLuint shader)`
- **Description**: Compiles a shader object
- **Parameters**:
  - `shader`: Shader object to compile
- **Returns**: void

#### `glCreateProgram()`
- **Description**: Creates a program object
- **Returns**: `GLuint` program ID or 0 on failure

#### `glAttachShader(GLuint program, GLuint shader)`
- **Description**: Attaches a shader object to a program object
- **Parameters**:
  - `program`: Program object
  - `shader`: Shader object
- **Returns**: void

#### `glLinkProgram(GLuint program)`
- **Description**: Links a program object
- **Parameters**:
  - `program`: Program object to link
- **Returns**: void

### Uniform Management

#### `glGetUniformLocation(GLuint program, const char* name)`
- **Description**: Returns the location of a uniform variable in a shader program
- **Parameters**:
  - `program`: Program object to query
  - `name`: Name of the uniform variable
- **Returns**: Location of the uniform variable or -1 if not found

#### `glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)`
- **Description**: Specifies value for a vec4 uniform variable
- **Parameters**:
  - `location`: Location of the uniform variable
  - `v0, v1, v2, v3`: Components of the vector (x, y, z, w)
- **Returns**: void

#### `glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)`
- **Description**: Specifies value for an ivec4 uniform variable
- **Parameters**:
  - `location`: Location of the uniform variable
  - `v0, v1, v2, v3`: Components of the integer vector
- **Returns**: void

### Color Management

#### `glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)`
- **Description**: Sets the clear color for the color buffer
- **Parameters**:
  - `red`: Red component (0.0 to 1.0)
  - `green`: Green component (0.0 to 1.0)
  - `blue`: Blue component (0.0 to 1.0)
  - `alpha`: Alpha component (0.0 to 1.0)
- **Returns**: void

### Rendering

#### `glClear(GLbitfield mask)`
- **Description**: Clears buffers to preset values
- **Parameters**:
  - `mask`: Bitwise OR of buffers to clear (e.g., GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
- **Returns**: void

#### `glUseProgram(GLuint program)`
- **Description**: Installs a program object as part of current rendering state
- **Parameters**:
  - `program`: Program object to use
- **Returns**: void

#### `glDrawArrays(GLenum mode, GLint first, GLsizei count)`
- **Description**: Renders primitives from array data
- **Parameters**:
  - `mode`: Type of primitives to render (e.g., GL_TRIANGLES)
  - `first`: Starting index
  - `count`: Number of vertices to render
- **Returns**: void

## Renderer Class API

### Constructor/Destructor

#### `Renderer()`
- **Description**: Initializes renderer with null/zero values
- **Returns**: Renderer instance

#### `~Renderer()`
- **Description**: Cleans up OpenGL resources
- **Returns**: void

#### `Renderer(int width, int height)`
- **Description**: Initializes renderer with specified window dimensions
- **Parameters**:
  - `width`: Window width in pixels
  - `height`: Window height in pixels
- **Returns**: Renderer instance

### Public Methods

#### `bool init()`
- **Description**: Initializes OpenGL context and buffers
- **Returns**: `true` on success, `false` on failure

#### `bool loadShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)`
- **Description**: Loads and compiles shader programs
- **Parameters**:
  - `vertexShaderPath`: Path to vertex shader file
  - `fragmentShaderPath`: Path to fragment shader file
- **Returns**: `true` on success, `false` on failure

#### `void render()`
- **Description**: Renders the scene
- **Returns**: void

#### `void cleanup()`
- **Description**: Cleans up OpenGL resources
- **Returns**: void

### Private Methods

#### `std::string loadShader(const std::string& filePath)`
- **Description**: Loads shader source from file
- **Parameters**:
  - `filePath`: Path to shader file
- **Returns**: String containing shader source code

#### `void checkShaderCompileErrors(GLuint shader, const std::string& type)`
- **Description**: Checks for shader compilation errors
- **Parameters**:
  - `shader`: Shader object to check
  - `type`: Type of shader for error reporting
- **Returns**: void

### Private Members

#### `GLint colorUniformLocation`
- **Description**: Location of the triangle color uniform variable in the shader program
- **Type**: `GLint`

#### `float time`
- **Description**: Time tracking variable for animations
- **Type**: `float`

#### `int windowWidth`
- **Description**: Width of the window in pixels
- **Type**: `int`

#### `int windowHeight`
- **Description**: Height of the window in pixels
- **Type**: `int` 