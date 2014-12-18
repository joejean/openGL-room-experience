#version 330 core

in vec2 s_fTexcoord;
out vec4 fColor;

uniform sampler2D diffuseMap;//GLTEXTURE0

void main () {
    //fColor=vec4 (1,1,1,1);
	fColor = texture(diffuseMap, s_fTexcoord);
}