#version 330 core
out vec4 FragColor;

uniform vec4 shapeColor;  // Color for all shapes

void main()
{
    FragColor = shapeColor;  // Use the uniform color for the shape
} 