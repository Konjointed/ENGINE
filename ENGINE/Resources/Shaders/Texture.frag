#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    float gray = texture(texture1, TexCoord).r; // Extract the red channel
    FragColor = vec4(gray, gray, gray, 1.0);   // Replicate it across RGB
}
