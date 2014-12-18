#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;

uniform sampler2D sampler;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shininess;
uniform bool Kd_map_present;

out vec4 color;

vec4 computeColor();

void main(){   
	color = computeColor();
}

vec4 computeColor(){
	vec3 lightPosition = vec3(0,0,-2);
	vec3 eyePosition = vec3(0,0,-2);
	vec3 ambientIntensity = 0.1*vec3(1,1,1);
	vec3 diffuseIntensity = vec3(0.9,0.9,0.9);
	vec3 specularIntensity = vec3(0.7,0.7, 0.7);

	vec3 ambient = Ka;
	vec3 diffuse = Kd;
	vec3 specular = Ks;
	float sh = 64; //shininess;

	
	if(Kd_map_present){
		diffuse*=texture(sampler, texCoord0);
	}

    //return diffuse;
    
	vec3 dirToLight = normalize(lightPosition - position0);
	vec3 dirToEye = normalize(eyePosition - position0);
	vec3 halfvector = normalize(dirToEye+dirToLight);
	float cosSigma = dot(halfvector, normal0);
	float cosTheta = clamp(dot(normal0, dirToLight),0,1);
	
	vec3 col =  ambientIntensity*ambient + diffuseIntensity*diffuse*cosTheta + specularIntensity*specular*pow(cosSigma,sh);

	return vec4(col,1.0);
    
}