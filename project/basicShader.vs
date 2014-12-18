#version 330


uniform mat4 Mcam;
uniform mat4 Mproj;

in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 texCoord0;
out vec3 normal0;

out vec3 position0;

uniform mat4 MVP;
uniform mat4 Normal;

uniform float time;

void main()
{
	float t = mod(time,10)/10; t*=2*3.14159;
	//mat3 M = mat3( cos(t), 0 , sin(t), 0,  1,  0, -sin(t), 0,  cos(t) );

//	position0 = M*position;
	
	position0 = position.xyz;
	
//	normal0 = normalize(M*normal);
	normal0 = normal;

	texCoord0 = texCoord;

	//gl_Position = Mproj*Mcam*vec4(position0, 1.0);
	gl_Position = position;
	
}