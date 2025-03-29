#version 330 core

layout(location = 0) in vec3 aPos; // Vertex position
layout(location = 1) in vec3 aColor; // Vertex color

out vec3 vertexColor; // Output color to fragment shader

uniform mat4 model; // Model transformation matrix
uniform mat4 view; // View transformation matrix
uniform mat4 projection; // Projection transformation matrix

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = aColor; // Pass color to fragment shader
}