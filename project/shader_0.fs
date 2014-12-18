#version 330 core

//Bug report

/****
(From the normal map)The light is the strongest when the normal is at biggest angle with light direction
suspect the normal map has some weird values

******/

in vec2 s_fTexcoord;
in vec3 s_fPosition;
//in vec3 fnewEyePosition;

uniform vec3 camPos;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform mat4 Mcam;
uniform mat4 Mproj;

uniform vec3 gaze;

out vec4 fColor;

vec4 diffuseColor;

vec4 computeColor();

vec4 spotLightColor();

vec3 position;
vec3 theNormal;
void main () {

	position=s_fPosition;

	vec4 tColor = computeColor()+spotLightColor();
	//fColor = computeColor()+spotLightColor();

	//float length= sqrt(tColor.x*tColor.x,tColor.y*tColor.y,tColor.z*tColor.z);
	float l=length(tColor.xyz);
	l=pow(l,0.1);

	fColor=vec4(tColor.xyz/l,1);

	//fColor=spotLightColor();
	//fColor = computeColor();


}	

//compute the specular phong lighting color + diffuse lighting color
vec4 computeColor(){
	
	vec4 light = vec4(0.8,0.8,0.8,1.0);
	vec3 eyePosition = camPos;
	vec3 lightPosition = vec3(0,2,0); 
	  
	// set new color
	vec4 ambient = vec4(0.1,0.1, 0.1, 1);

	diffuseColor = texture(diffuseMap, s_fTexcoord);       // Get the diffuse color from texture map 0
	vec4 normalMapColor = texture(normalMap, s_fTexcoord); // Get the color from texture map 1  
	vec3 normal = 2*normalMapColor.xyz - vec3(1,1,1);      // convert the color to normal vector   
	//theNormal=normal;


	vec4 h_normal=vec4(normal,0);
	
	//normal=(transpose(inverse(Mproj*Mcam))*h_normal).xyz;

    normal=(Mproj*Mcam*h_normal).xyz;


	normal= normalize(normal);
    normal.z = -normal.z;                                  // invert z direction since we are using Normalized Device Coordinates 


	//normal=vec3(0,1,0);
	theNormal=normal;
	
	vec4 specular = vec4(1,1,1,1);
	float shininess = 10;

	vec3 dirToLight = normalize(lightPosition - position);
	vec3 dirToEye = normalize(eyePosition - position);
	vec3 halfvector = normalize(dirToEye+dirToLight);
	float cosSigma = dot(halfvector, normal);
	float cosTheta = clamp(dot(normal, dirToLight),0,1);
	return ambient*diffuseColor + diffuseColor*cosTheta + specular*pow(cosSigma,shininess);
    //return vec4(0,0,0,1);
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
