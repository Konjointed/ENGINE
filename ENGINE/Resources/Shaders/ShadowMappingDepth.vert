#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform mat4 finalBonesMatrices[100]; // Adjust the size according to your needs

void main()
{
    vec4 totalPosition = vec4(aPos, 1.0);
    vec4 skinnedPosition = vec4(0.0);

    for(int i = 0; i < 4; ++i) // Assuming each vertex is affected by up to 4 bones
    {
        if(boneIds[i] != -1)
        {
            skinnedPosition += finalBonesMatrices[boneIds[i]] * totalPosition * weights[i];
        }
    }

    gl_Position = lightSpaceMatrix * model * skinnedPosition;
}
