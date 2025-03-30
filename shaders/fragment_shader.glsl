#version 330 core
out vec4 FragColor;

uniform vec4 triangleColor;  // Add uniform variable for color

void main()
{
    FragColor = triangleColor;  // Use the uniform color instead of hardcoded value
}