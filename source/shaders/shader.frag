#version 330 core

// Interpolated values from the vertex shaders
in vec4 fragColor;

// output data
out vec4 color;

void main()
{
    // Output color = color specified in the vertex shader,
    // interpolated between all 3 surrounding vertices of the triangle
    color = fragColor * fragColor.a;
}
