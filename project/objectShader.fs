#version 330


uniform mat4 Mcam;
uniform mat4 Mproj;


uniform vec3 gaze;

uniform vec3 camPos;

in vec2 texCoord0;
in vec3 normal0;
in vec3 s_fposition;

vec3 position;

vec3 theNormal;

vec4 diffuseColor;

uniform sampler2D sampler;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shininess;
uniform bool Kd_map_present;

out vec4 color;

vec4 computeColor();
vec4 spotLightColor();

void main(){   
    position=s_fposition;
    theNormal=normal0;
//	color = computeColor()+spotLightColor();
    color = computeColor();
}

vec4 computeColor(){


	vec3 lightPosition = vec3(0,10,0);
	vec3 eyePosition = camPos;
	vec3 ambientIntensity = 0.1*vec3(1,1,1);
	vec3 diffuseIntensity = vec3(0.8,0.8,0.8);
	vec3 specularIntensity = vec3(0.7,0.7, 0.7);

	vec3 ambient = Ka;
	vec3 diffuse = Kd;
	vec3 specular = Ks;
	float sh = 64; //shininess;

	
	if(Kd_map_present){
		diffuse*=texture(sampler, texCoord0);
	}

    diffuseColor=vec4(diffuse,1.0);

    return diffuse;
   
	vec3 dirToLight = normalize(lightPosition - s_fposition);
	vec3 dirToEye = normalize(eyePosition - s_fposition);
	vec3 halfvector = normalize(dirToEye+dirToLight);
	float cosSigma = dot(halfvector, normal0);
	float cosTheta = clamp(dot(normal0, dirToLight),0,1);
	
	vec3 col =  ambientIntensity*ambient + diffuseIntensity*diffuse*cosTheta + specularIntensity*specular*pow(cosSigma,sh);

	//return vec4(col,1.0);
    
}




vec4 spotLightColor(){
	
	vec3 axis = gaze; 

	float angle = 3.14159/10;
	
	vec3 lightPosition = camPos; 
	vec3 dirToLight = normalize(lightPosition - position);

	float cosAngleFromAxis = dot(-dirToLight, axis);
	float cosAngleLimit = cos(angle);
	

	float intensity = cosAngleFromAxis<cosAngleLimit ? 0: pow(cosAngleFromAxis,100);

	
	vec3 eyePosition = camPos;


	vec3 dirToEye = normalize(eyePosition - position);
	
	
	float dist=clamp((eyePosition-position).x/dirToEye.x/5,0,1);

	intensity = intensity/(dist*dist);

	vec3 halfvector = normalize(dirToLight);
	
	float cosSigma = dot(halfvector, theNormal);

	vec4 specular = vec4(0.5,0.5,0.5,1);

	float shininess = 1;

	float cosTheta = clamp(dot(theNormal, dirToLight),0,1);
	return intensity*diffuseColor*cosTheta+intensity * specular*pow(cosSigma,shininess);
	//return intensity*diffuseColor*cosTheta;
	//return intensity*diffuseColor;
}
