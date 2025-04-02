#version 330 core
uniform vec4 shapeColor;
out vec4 FragColor;

void main() {
    // Get the distance from the center of the screen
    vec2 center = vec2(0.0, 0.0);
    vec2 pos = gl_FragCoord.xy / vec2(800.0, 600.0) * 2.0 - 1.0;
    float dist = length(pos - center);
    
    // Draw a circle with smooth edges
    float radius = 0.5;
    float smoothness = 0.01;
    float alpha = smoothstep(radius, radius - smoothness, dist);
    
    // Apply the color with the calculated alpha
    FragColor = vec4(shapeColor.rgb, shapeColor.a * alpha);
} 