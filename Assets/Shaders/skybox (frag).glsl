#version 460 core

in vec3 TexCords;

out vec4 color;

uniform samplerCube skybox;

void main() {
	color = texture(skybox, TexCords);

}
