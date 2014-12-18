#version 330



//layout(location = 0) in vec4 position;
in vec4 position;

//layout(location = 1) in vec2 texCoord;
in vec2 texCoord;

layout(location = 2) in vec4 normal;
//in vec3 normal;

uniform mat4 Mproj;

uniform mat4 Mcam;


mat4 activateUniforms;
vec3 activateGaze;

uniform mat4 MVP;
uniform mat4 Normal;

uniform float time;

//uniform mat4 time;


out vec2 texCoord0;
out vec3 normal0;
out vec3 s_fposition;

vec4 tPosition;
mat4 Tmat;

void main()
{
	mat4 Tmat;//translation matrix
	Tmat[0][0]=1;
	Tmat[1][1]=1;
	Tmat[2][2]=1;
	Tmat[3][3]=1;
	Tmat[3][1]=-5;

//	Tmat=normalize(Tmat);

	activateUniforms = Mproj;
	
	activateUniforms = Mcam;

	//activateGaze = gaze;

	tPosition= position;

	float t = mod(time,10)/10; t*=2*3.14159;
	//mat3 M = mat3( cos(t), 0 , sin(t), 0,  1,  0, -sin(t), 0,  cos(t) );

//	position0 = M*position;
	
//	tPosition.normalize(5.0);

	s_fposition = tPosition.xyz;
	
//	mat4 = 

	//normal0 = normalize(M*normal).xyz;
	//normal0 =(Mproj*Mcam*normal).xyz;
	normal0 = (Mproj*Mcam*normal).xyz;

	texCoord0 = texCoord;

	//gl_Position = Mproj*Mcam*position;
	//gl_Position = position;
	gl_Position =  Mproj*Mcam*Tmat*tPosition;
		
}