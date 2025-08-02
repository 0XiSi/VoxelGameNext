//Vertex Shader
#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 3) in vec2 tileOffset;

out vec2 TexCoord;
flat out vec3 Normal;
out vec3 FragPos;
out vec4 DirectionalLightSpacePos;
out vec2 TileOffset;

//The flat keyword should be added before out vec3 Normal
//If we use "flat" keyword it means that instead of interpolating the normal it will keep it consistant everywhere.

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;

void main()												
{														
	gl_Position = projection * view * model * vec4(pos, 1.0f);	
	DirectionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0); //The information for where the object is relative to the light.
	
	TexCoord = tex;
	TileOffset = tileOffset;

	Normal = mat3(transpose(inverse(model))) * norm;
	
	FragPos = (model * vec4(pos, 1.0f)).xyz;
}