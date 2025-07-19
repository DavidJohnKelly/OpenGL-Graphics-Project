#version 330 core
out vec4 FragColor;

uniform bool selected;

void main()
{
    // Simply alternate between red and green for selected or not
    // Transparency allows seeing of object beneath
    FragColor = vec4(1.0f - float(selected), 0.0f + float(selected), 0.0f, 0.15f);
}