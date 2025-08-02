#version 460 core

layout (location = 0) in vec3 pos;

out vec3 TexCords;

uniform mat4 projection;
uniform mat4 view;

void main() 
{
	TexCords = pos;
	gl_Position = projection * view * vec4(pos, 1.0);
}