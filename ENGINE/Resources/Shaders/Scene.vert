#version 330 core 

layout (location = 0) in vec3 aPosition; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

// Note: Put in skinning shader
//layout (location = 5) in ivec4 boneIds;
//layout (location = 6) in vec4 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec2 fTexCoord;
out vec3 fPosition;
out vec3 fNormal;
out vec3 fTangent;
out vec3 fBitangent;
out vec4 fPosLightSpace;

void main() {
	fTexCoord = aTexCoord;
	fPosition = vec3(model * vec4(aPosition, 1.0));
	fNormal = transpose(inverse(mat3(model))) * aNormal;
	fPosLightSpace = lightSpaceMatrix * vec4(fPosition, 1.0);

	gl_Position = projection * view * model * vec4(aPosition, 1.0);
}