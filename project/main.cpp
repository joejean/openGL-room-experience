#include <iostream>
#include <fstream>
#include <sstream>
#include "texture.h"
#include "LoadShaders.h"


#include "includes.h"
#include "Camera.h"

using namespace std;

#define BUFFER_OFFSET(offset) ( (void *) offset )
#define EPS 0.000000001
#define PI 3.14159265359

//Assimp::Importer importer;

//GLuint spotLightSwitch

GLuint Mcam_unif;
GLuint Mproj_unif;
GLuint gaze_unif;//send the gaze vector to fshader to do spot lighting

bool draw_box = false;

GLuint camPos;

Camera cam;

glm::vec3 camPosition, gaze, up;

glm::vec2 mouseRotate;

//GLuint shaderProgramID; //replaced with GLuint program


//the rotate matrix passed to the vertex shader each time
GLfloat rotate_mat[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

//matrix to store the rotation from earlier
GLfloat old_rotate_mat[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

int g_height = 512;
int g_width = 512;



GLfloat p1_x, p1_y, p2_x, p2_y;

GLuint program_0;
GLuint program_1;


GLuint program_object;
GLuint program_box;


Texture tex0,tex1,tex2,tex3,texBox;

/**********COPY CODE************/

/*END COPY CODE*/


/****************COPY CODE*****************************/

enum VAO_IDs { TopFaces,BottomFace,smallBox, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

//dimensions for the cube
//GLfloat s = 5.0f;


GLfloat nearPlane = -0.1;
GLfloat farPlane = -5000;

GLfloat boxSize = 2.0f;

GLfloat boxVertices[] = {
	//front -- 123, 134
	boxSize, boxSize, boxSize, -boxSize, boxSize, boxSize, -boxSize, -boxSize, boxSize,
	boxSize, boxSize, boxSize, -boxSize, -boxSize, boxSize, boxSize, -boxSize, boxSize,

	//back -- 658,687
	-boxSize, boxSize, -boxSize, boxSize, boxSize, -boxSize, boxSize, -boxSize, -boxSize,
	-boxSize, boxSize, -boxSize, boxSize, -boxSize, -boxSize, -boxSize, -boxSize, -boxSize,

	//left -- 267,273
	-boxSize, boxSize, boxSize, -boxSize, boxSize, -boxSize, -boxSize, -boxSize, -boxSize,
	-boxSize, boxSize, boxSize, -boxSize, -boxSize, -boxSize, -boxSize, -boxSize, boxSize,

	//rgiht -- 514,548
	boxSize, boxSize, -boxSize, boxSize, boxSize, boxSize, boxSize, -boxSize, boxSize,
	boxSize, boxSize, -boxSize, boxSize, -boxSize, boxSize, boxSize, -boxSize, -boxSize,
	//top -- 562,521
	boxSize, boxSize, -boxSize, -boxSize, boxSize, -boxSize, -boxSize, boxSize, boxSize,
	boxSize, boxSize, -boxSize, -boxSize, boxSize, boxSize, boxSize, boxSize, boxSize,

	//bottom -- 437,478
	boxSize, -boxSize, boxSize, -boxSize, -boxSize, boxSize, -boxSize, -boxSize, -boxSize,
	boxSize, -boxSize, boxSize, -boxSize, -boxSize, -boxSize, boxSize, -boxSize, -boxSize
};


GLfloat boxTexcoords[] = {
	//front -- 123, 134
	1 / 2.0f, 1 / 3.0f, 1 / 4.0f, 1 / 3.0f, 1 / 4.0f, 2 / 3.0f,
	1 / 2.0f, 1 / 3.0f, 1 / 4.0f, 2 / 3.0f, 1 / 2.0f, 2 / 3.0f,
	//back -- 658,687
	1.0f, 1 / 3.0f, 3 / 4.0f, 1 / 3.0f, 3 / 4.0f, 2 / 3.0f,
	1.0f, 1 / 3.0f, 3 / 4.0f, 2 / 3.0f, 1.0f, 2 / 3.0f,

	//left -- 267,273
	1 / 4.0f, 1 / 3.0f, 0.0f, 1 / 3.0f, 0.0f, 2 / 3.0f,
	1 / 4.0f, 1 / 3.0f, 0.0f, 2 / 3.0f, 1 / 4.0f, 2 / 3.0f,
	//rgiht -- 514,548
	3 / 4.0f, 1 / 3.0f, 1 / 2.0f, 1 / 3.0f, 1 / 2.0f, 2 / 3.0f,
	3 / 4.0f, 1 / 3.0f, 1 / 2.0f, 2 / 3.0f, 3 / 4.0f, 2 / 3.0f,
	//top -- 562,521
	1 / 2.0f, 0.0f, 1 / 4.0f, 0.0f, 1 / 4.0f, 1 / 3.0f,
	1 / 2.0f, 0.0f, 1 / 4.0f, 1 / 3.0f, 1 / 2.0f, 1 / 3.0f,
	//bottom -- 437,478
	1 / 2.0f, 2 / 3.0f, 1 / 4.0f, 2 / 3.0f, 1 / 4.0f, 1.0f,
	1 / 2.0f, 2 / 3.0f, 1 / 4.0f, 1.0f, 1 / 2.0f, 1.0f
};




//NumVertices = number of faces * 3;
const GLuint NumVertices = 30;

/*
GLfloat vertices[] = {
	//front -- 123, 134
	s,s,s,        -s,s,s,        -s,-s,s,
	s,s,s,        -s,-s,s,        s,-s,s,

//back -- 658,687
	-s,s,-s,       s,s,-s,        s,-s,-s,
	-s,s,-s,       s,-s,-s,      -s,-s,-s,

//left -- 267,273
	-s,s,s,        -s,s,-s,       -s,-s,-s,
	-s,s,s,        -s,-s,-s,      -s,-s,s,

//rgiht -- 514,548
	s,s,-s,         s,s,s,        s,-s,s,
	s,s,-s,         s,-s,s,       s,-s,-s,
//top -- 562,521
	s,s,-s,         -s,s,-s,      -s,s,s,
	s,s,-s,         -s,s,s,       s,s,s,
//bottom -- 437,478
	//s,-s,s,         -s,-s,s,      -s,-s,-s,
	//s,-s,s,         -s,-s,-s,      s,-s,-s
};

GLfloat texcoords[] = {
	//front -- 123, 134
	1/2.0f,1/3.0f,        1/4.0f,1/3.0f,       1/4.0f,2/3.0f,
	1/2.0f,1/3.0f,        1/4.0f,2/3.0f,       1/2.0f,2/3.0f,
	//back -- 658,687
	1.0f,1/3.0f,          3/4.0f,1/3.0f,       3/4.0f,2/3.0f,
	1.0f,1/3.0f,          3/4.0f,2/3.0f,       1.0f,2/3.0f,

	//left -- 267,273
	1/4.0f,1/3.0f,         0.0f,1/3.0f,        0.0f,2/3.0f,
	1/4.0f,1/3.0f,         0.0f,2/3.0f,        1/4.0f,2/3.0f,
	//rgiht -- 514,548
	3/4.0f,1/3.0f,         1/2.0f,1/3.0f,      1/2.0f,2/3.0f,
	3/4.0f,1/3.0f,         1/2.0f,2/3.0f,      3/4.0f,2/3.0f,
	//top -- 562,521
	1/2.0f, 0.0f,          1/4.0f, 0.0f,       1/4.0f, 1/3.0f,
	1/2.0f, 0.0f,          1/4.0f, 1/3.0f,     1/2.0f, 1/3.0f,
	//bottom -- 437,478
	//1/2.0f, 2/3.0f,        1/4.0f, 2/3.0f,     1/4.0f, 1.0f,
	//1/2.0f, 2/3.0f,        1/4.0f, 1.0f,       1/2.0f, 1.0f
};


//coordinates for the bottom surface, rendered separtately from the rest
const GLuint NumVerticesBot = 6;

GLfloat verticesBot[] = {
	s, -s, s, -s, -s, s, -s, -s, -s,
	s, -s, s, -s, -s, -s, s, -s, -s
};
GLfloat texcoordsBot[] = {
	0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f
};

*/

GLfloat s = 20.0f;

GLfloat w = 20.0f;
GLfloat h = 20.0f;
GLfloat l = 20.0f;

//NumVertices = number of faces * 3;
//const GLuint NumVertices = 30;

GLfloat vertices[][3] = {
	//front -- 123, 134
	w, h, l, -w, h, l, -w, -h, l,
	w, h, l, -w, -h, l, w, -h, l,

	//back -- 658,687
	-w, h, -l, w, h, -l, w, -h, -l,
	-w, h, -l, w, -h, -l, -w, -h, -l,

	//left -- 267,273
	-w, h, l, -w, h, -l, -w, -h, -l,
	-w, h, l, -w, -h, -l, -w, -h, l,

	//rgiht -- 514,548
	w, h, -l, w, h, l, w, -h, l,
	w, h, -l, w, -h, l, w, -h, -l,
	//top -- 562,521
	w, h, -l, -w, h, -l, -w, h, l,
	w, h, -l, -w, h, l, w, h, l,
	//bottom -- 437,478
	w, -h, l, -w, -h, l, -w, -h, -l,
	w, -h, l, -w, -h, -l, w, -h, -l
};

GLfloat texcoords[] = {
	//front -- 123, 134
	1 / 2.0f, 1 / 3.0f, 1 / 4.0f, 1 / 3.0f, 1 / 4.0f, 2 / 3.0f,
	1 / 2.0f, 1 / 3.0f, 1 / 4.0f, 2 / 3.0f, 1 / 2.0f, 2 / 3.0f,
	//back -- 658,687
	1.0f, 1 / 3.0f, 3 / 4.0f, 1 / 3.0f, 3 / 4.0f, 2 / 3.0f,
	1.0f, 1 / 3.0f, 3 / 4.0f, 2 / 3.0f, 1.0f, 2 / 3.0f,

	//left -- 267,273
	1 / 4.0f, 1 / 3.0f, 0.0f, 1 / 3.0f, 0.0f, 2 / 3.0f,
	1 / 4.0f, 1 / 3.0f, 0.0f, 2 / 3.0f, 1 / 4.0f, 2 / 3.0f,
	//rgiht -- 514,548
	3 / 4.0f, 1 / 3.0f, 1 / 2.0f, 1 / 3.0f, 1 / 2.0f, 2 / 3.0f,
	3 / 4.0f, 1 / 3.0f, 1 / 2.0f, 2 / 3.0f, 3 / 4.0f, 2 / 3.0f,
	//top -- 562,521
	1 / 2.0f, 0.0f, 1 / 4.0f, 0.0f, 1 / 4.0f, 1 / 3.0f,
	1 / 2.0f, 0.0f, 1 / 4.0f, 1 / 3.0f, 1 / 2.0f, 1 / 3.0f,
	//bottom -- 437,478
	//1/2.0f, 2/3.0f,        1/4.0f, 2/3.0f,     1/4.0f, 1.0f,
	//1/2.0f, 2/3.0f,        1/4.0f, 1.0f,       1/2.0f, 1.0f
};


//coordinates for the bottom surface, rendered separtately from the rest
const GLuint NumVerticesBot = 6;


GLfloat verticesBot[] = {
	w, -h, l, -w, -h, l, -w, -h, -l,
	w, -h, l, -w, -h, -l, w, -h, -l,
};

GLfloat texcoordsBot[] = {
	0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f
};

/*********
Load Object Stuff
**********/

GLuint time_unif;

Model *m;


void init_camera_object();
void init_camera_box();
void update_camera_for_all();

void init_box(){

	ShaderInfo shader_box = { GL_VERTEX_SHADER, "shader_box.vs", GL_FRAGMENT_SHADER, "shader_box.fs" };
	program_box = LoadShaders(shader_box);


	glUseProgram(program_box);

	init_camera_box();

	glGenVertexArrays(1, &VAOs[smallBox]);
	glBindVertexArray(VAOs[smallBox]);

	glGenBuffers(NumBuffers, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);


	//Create the buffer but don't load anything
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(boxVertices) + sizeof(boxTexcoords),
		NULL,
		GL_STATIC_DRAW);

	//Load the vertex data
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeof(boxVertices),
		boxVertices);

	//Load the colors data right after that
	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(boxVertices),
		sizeof(boxTexcoords),
		boxTexcoords);

	GLuint vPosition = glGetAttribLocation(program_box, "s_vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vTexcoord = glGetAttribLocation(program_box, "s_vTexcoord");
	glEnableVertexAttribArray(vTexcoord);
	glVertexAttribPointer(vTexcoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(boxVertices)));
	

	//! !!!!!!!!!!!!!!!!!! THIS IS IMPORTANT BASED ON WHERE YOU LOAD THE TEXTURE
	glUniform1i(glGetUniformLocation(program_box, "diffuseMap"), 30); // set the variable diffuseMap to 0 so that it uses texture0

}

Model *m2;
void init_object(void){

	ShaderInfo shader_object = { GL_VERTEX_SHADER, "objectShader.vs", GL_FRAGMENT_SHADER, "objectShader.fs" };
	program_object = LoadShaders(shader_object);


	printf("init_object");

	init_camera_object();

	//m = new Model(program,"Objects/pear/pear.obj", "Objects/pear/", false, true, 1);
	//m = new Model(program,"Objects/uh60/uh60.obj", "Objects/uh60/", false, true, 1);


	//m = new Model(program,"Objects/Starship/Starship.obj", "Objects/Starship/", false, true, 1);
	//m = new Model(program,"Objects/Rifle/Rifle.obj", "Objects/Rifle/", false, true, 1);
	//m = new Model(program,"Objects/dpv/dpv.obj", "Objects/dpv/", false, true, 1);
	//m = new Model(program,"Objects/ferrari_599gtb/ferrari_599gtb.obj", "Objects/ferrari_599gtb/", false, true, 1);
	//m = new Model(program, "Objects/zombie/zombie.obj", "Objects/zombie/", false, true, 1);
	m = new Model(program_object, "Objects/Swan/Swan.obj", "Objects/Swan/", false, true, 10);
	//m = new Model(program, "Objects/Dog/ZombiDog.obj", "Objects/Dog/", false, true, 1);
	//m = new Model(program, "Objects/Nurseknife/Nurse.obj", "Objects/Nurseknife/", false, true,10);
	//m = new Model(program, "Objects/Policeman/Policeman.obj", "Objects/Policeman/", false, true,1);

}


/********
END LOAD OBJECT STUFF
*********/




void mouse(int button, int state, int xx, int yy){
	// normalize xx and yy to x and y
	GLfloat x = 2 * (GLfloat)xx / g_width - 1;
	GLfloat y = 1 - 2 * (GLfloat)yy / g_width;
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			p1_x = x; p1_y = y;
		}
		else if (state == GLUT_UP){
			//printf("Mouse Up Function");
			p1_x = EPS; p1_y = EPS;


		}
	}
}

void mouseMotion(int xx, int yy){

	//printf("X: %d", xx);
	//printf("Y: %d\n", yy);

	GLfloat x = 2 * (GLfloat)xx / g_width - 1;
	GLfloat y = 1 - 2 * (GLfloat)yy / g_width;

	x = x*PI;
	y = y*PI;

	glm::vec3 o_gaze = glm::vec3(0, 0, -1);
	glm::vec3 o_up = glm::vec3(0, 1, 0);
	glm::vec3 o_w = glm::cross(-o_gaze, o_up);

	cam.gaze = cos(y)*o_gaze + sin(y)*o_up;
	cam.up = cos(y)*o_up - sin(y)*o_gaze;

	//temperory cam parameters
	glm::vec3 t_gaze = cam.gaze;
	glm::vec3 t_up = cam.up;
	glm::vec3 t_w = glm::cross(-cam.gaze, cam.up);

	cam.gaze = cos(x)*t_gaze - sin(x)*t_w;

	update_camera_for_all();

	glutPostRedisplay();
}


void special(int key, int x, int y)
{

	float theta = 0.05f;

	glm::vec3 old_cam_gaze = cam.gaze;
	glm::vec3 old_cam_up = cam.up;
	glm::vec3 old_cam_w = glm::cross(-cam.gaze, cam.up);

	switch (key)
	{
	case GLUT_KEY_UP:

		cam.gaze = cos(theta)*old_cam_gaze + sin(theta)*old_cam_up;
		cam.up = cos(theta)*old_cam_up - sin(theta)*old_cam_gaze;
		
		update_camera_for_all();

		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:

		cam.gaze = cos(theta)*old_cam_gaze - sin(theta)*old_cam_up;
		cam.up = cos(theta)*old_cam_up + sin(theta)*old_cam_gaze;
		
		update_camera_for_all();

		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		cam.gaze = cos(theta)*old_cam_gaze + sin(theta)*old_cam_w;

		update_camera_for_all();

		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:

		cam.gaze = cos(theta)*old_cam_gaze - sin(theta)*old_cam_w;
		update_camera_for_all();

		glutPostRedisplay();
		break;
	}
}

void keyboard(unsigned char key, int x, int y){

	glm::vec3 side = 0.1f*glm::cross(cam.gaze, cam.up);

	float next_x = cam.position.x + 0.1f*cam.gaze.x;
	float next_y = cam.position.y + 0.1f*cam.gaze.y;
	float next_z = cam.position.z + 0.1f*cam.gaze.z;

	float prev_x = cam.position.x - 0.1f*cam.gaze.x;
	float prev_y = cam.position.y - 0.1f*cam.gaze.y;
	float prev_z = cam.position.z - 0.1f*cam.gaze.z;

	float side_right_x = cam.position.x - side.x;
	float side_right_y = cam.position.y - side.y;
	float side_right_z = cam.position.z - side.z;

	float side_left_x = cam.position.x + side.x;
	float side_left_y = cam.position.y + side.y;
	float side_left_z = cam.position.z + side.z;


	if (key == 'w'){
		if (next_x > -(s - 2) && next_x < (s - 2) &&
			next_y > -(s - 2) && next_y < (s - 2) &&
			next_z > -(s - 2) && next_z < (s - 2)
			){
			cam.position.x += 0.1f*cam.gaze.x;
			cam.position.z += 0.1f*cam.gaze.z;

			update_camera_for_all();

			glutPostRedisplay();
		}
	}
	else if (key == 's'){
		if (prev_x > -(s - 2) && prev_x < (s - 2) &&
			prev_y > -(s - 2) && prev_y < (s - 2) &&
			prev_z > -(s - 2) && prev_z < (s - 2)
			){
			cam.position.x -= 0.1f*cam.gaze.x;
			cam.position.z -= 0.1f*cam.gaze.z;

			update_camera_for_all();


			glutPostRedisplay();
		}
	}
	else if (key == 'a'){
		if (side_right_x > -(s - 2) && side_right_x < (s - 2) &&
			side_right_x > -(s - 2) && side_right_x < (s - 2) &&
			side_right_x > -(s - 2) && side_right_x < (s - 2)
			){

			cam.position.x -= side.x;
			cam.position.z -= side.z;

			update_camera_for_all();


			glutPostRedisplay();
		}
	}
	else if (key == 'd'){
		if (side_left_x > -(s - 2) && side_left_x < (s - 2) &&
			side_left_x > -(s - 2) && side_left_x < (s - 2) &&
			side_left_x > -(s - 2) && side_left_x < (s - 2)
			){

			cam.position.x += side.x;
			cam.position.z += side.z;

			glUseProgram(program_0);
			cam.setCameraMatrix();
			glUseProgram(program_1);
			cam.setCameraMatrix();

			glUseProgram(program_object);
			cam.setCameraMatrix();


			glutPostRedisplay();
		}
	}
}

void update_camera_for_all(){

	glUseProgram(program_0);
	cam.setCameraMatrix();
	glUseProgram(program_1);
	cam.setCameraMatrix();
	glUseProgram(program_object);
	cam.setCameraMatrix();
};



void loadTextures(void){

	glActiveTexture(GL_TEXTURE19);
	tex2.Load("cobblestone.jpg");
	tex2.Bind();

	glActiveTexture(GL_TEXTURE5);
	tex0.Load("cubemaplayout_1.png");
	tex0.Bind();

	glActiveTexture(GL_TEXTURE12);			// Make texture1 active
	tex1.Load("cobblestone_normal.jpg");	// Load texture from file
	tex1.Bind();

	glActiveTexture(GL_TEXTURE20);
	tex3.Load("cubemaplayout_normal.png");
	tex3.Bind();

	glActiveTexture(GL_TEXTURE30);  // MAKE SURE TO GIVE A DECENT LOCATION
	texBox.Load("Crate.png");   // WILL ALSO SNED THE PNG
	texBox.Bind();

}


void init_camera_box(){
	Mcam_unif = glGetUniformLocation(program_box, "Mcam");
	Mproj_unif = glGetUniformLocation(program_box, "Mproj");
	gaze_unif = glGetUniformLocation(program_box, "gaze");

	camPos = glGetUniformLocation(program_box, "camPos");

	GLuint vnewEyePos = glGetAttribLocation(program_box, "vnewEyePosition");
	glEnableVertexAttribArray(vnewEyePos);
	glVertexAttribPointer(vnewEyePos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	cam.setAttribLocations(Mcam_unif, Mproj_unif, gaze_unif);

	cam.position = glm::vec3(0, -2, 0);
	cam.gaze = glm::vec3(0, 0, -1);
	cam.up = glm::vec3(0, 1, 0);
	cam.setCameraMatrix();
	cam.setPerspectiveProjection(60, 1, nearPlane, farPlane);
};

void init_camera_top(){
	Mcam_unif = glGetUniformLocation(program_0, "Mcam");
	printf("Mcam_top: %d",Mcam_unif);
	Mproj_unif = glGetUniformLocation(program_0, "Mproj");
	gaze_unif = glGetUniformLocation(program_0, "gaze");


	camPos = glGetUniformLocation(program_0, "camPos");

	GLuint vnewEyePos = glGetAttribLocation(program_0, "vnewEyePosition");
	glEnableVertexAttribArray(vnewEyePos);
	glVertexAttribPointer(vnewEyePos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	cam.setAttribLocations(Mcam_unif, Mproj_unif, gaze_unif);

	cam.position = glm::vec3(0, 0, 0);
	cam.gaze = glm::vec3(0, 0, -1);
	cam.up = glm::vec3(0, 1, 0);
	cam.setCameraMatrix();
	cam.setPerspectiveProjection(60, 1, nearPlane, farPlane);
}

void init_camera_bottom(){
	Mcam_unif = glGetUniformLocation(program_1, "Mcam");
	printf("Mcam_bottom: %d", Mcam_unif);

	Mproj_unif = glGetUniformLocation(program_1, "Mproj");
	gaze_unif = glGetUniformLocation(program_1, "gaze");

	camPos = glGetUniformLocation(program_1, "camPos");

	//GLuint vnewEyePos = glGetAttribLocation(program_1, "vnewEyePosition");
	//glEnableVertexAttribArray(vnewEyePos);
	//glVertexAttribPointer(vnewEyePos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	cam.setAttribLocations(Mcam_unif, Mproj_unif, gaze_unif);

	cam.position = glm::vec3(0, 0, 0);
	cam.gaze = glm::vec3(0, 0, -1);
	cam.up = glm::vec3(0, 1, 0);
	cam.setCameraMatrix();
	cam.setPerspectiveProjection(60, 1, nearPlane, farPlane);
}

void init_camera_object(){


//	Mcam_unif = glGetUniformLocation(program_object, "Mcam");
	Mcam_unif = glGetUniformLocation(program_object, "Mcam");


	printf("Mcam_object: %d", Mcam_unif);

	time_unif = glGetUniformLocation(program_object, "time");
	printf("time_object: %d", time_unif);

//	time_unif = glGetUniformLocation(program_object, "Mcam");
//	printf("Mcam_object_by_time: %d", time_unif);


	Mproj_unif = glGetUniformLocation(program_object, "Mproj");
	
	printf("Mproj_unif_object: %d", Mproj_unif);

	
	//once added, spot light goes away
	//gaze_unif = glGetUniformLocation(program_object, "gaze");


	camPos = glGetUniformLocation(program_object, "camPos");

	//GLuint vnewEyePos = glGetAttribLocation(program_object, "vnewEyePosition");

	glUseProgram(program_object);

	//glEnableVertexAttribArray(vnewEyePos);
	//glVertexAttribPointer(vnewEyePos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	cam.setAttribLocations(Mcam_unif, Mproj_unif, gaze_unif);

	cam.position = glm::vec3(0, 0, 0);
	cam.gaze = glm::vec3(0, 0, -1);
	cam.up = glm::vec3(0, 1, 0);

	cam.setCameraMatrix();
	cam.setPerspectiveProjection(60, 1, nearPlane, farPlane);

	/*
	glm::vec3 theCamPos;
	GLfloat theCamPosX=1.0;
	GLfloat *theCamPosXPointer;
	theCamPosXPointer = &theCamPosX;
	glGetUniformfv(program_object, glGetUniformLocation(program_object, "camPos"), theCamPosXPointer);
	printf("f%",*theCamPosXPointer);
	*/	
}


void init_topFaces(void){

	//ShaderInfo shader_0 = { GL_VERTEX_SHADER, "shader_0.vs", GL_FRAGMENT_SHADER, "shader_0.fs" };
	//program_0 = LoadShaders(shader_0);
	//Shader shader0("myshader0");
	//program_0 = shader0.Bind();

	ShaderInfo shader_1 = { GL_VERTEX_SHADER, "shader_1.vs", GL_FRAGMENT_SHADER, "shader_1.fs" };
	program_0 = LoadShaders(shader_1);

	//glUseProgram(program_0);
	glUseProgram(program_0);

	init_camera_top();

	glGenVertexArrays(1, &VAOs[TopFaces]);
	glBindVertexArray(VAOs[TopFaces]);

	glGenBuffers(NumBuffers, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);


	//Create the buffer but don't load anything
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(vertices)+sizeof(texcoords),
		NULL,
		GL_STATIC_DRAW);

	//Load the vertex data
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeof(vertices),
		vertices);

	//Load the colors data right after that
	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(vertices),
		sizeof(texcoords),
		texcoords);

	GLuint vPosition = glGetAttribLocation(program_0, "s_vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vTexcoord = glGetAttribLocation(program_0, "s_vTexcoord");
	glEnableVertexAttribArray(vTexcoord);
	glVertexAttribPointer(vTexcoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	glUniform1i(glGetUniformLocation(program_0, "diffuseMap"), 0*7+5); // set the variable diffuseMap to 0 so that it uses texture0
	glUniform1i(glGetUniformLocation(program_0, "normalMap"),  20); // set the variable normalMap to 1 so that it uses texture1

	//****************END RENDER TOP 5 FACES

}

void init_bottomFaces()
{

	//*****************RENDER BOTTOM FACE
	//Use a new set of shaders for different uniform textures --- DOESNT WORK
	ShaderInfo shader_1 = { GL_VERTEX_SHADER, "shader_1.vs", GL_FRAGMENT_SHADER, "shader_1.fs" };
	program_1 = LoadShaders(shader_1);

	//Shader shader1("myshader1");
	//program_1 = shader1.Bind();

	glUseProgram(program_1);

	init_camera_bottom();

	glGenVertexArrays(1, &VAOs[BottomFace]);
	glBindVertexArray(VAOs[BottomFace]);

	glGenBuffers(NumBuffers, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);


	//Create the buffer but don't load anything
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(verticesBot) + sizeof(texcoordsBot),
		NULL,
		GL_STATIC_DRAW);

	//Load the vertex data
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeof(verticesBot),
		verticesBot);

	//Load the colors data right after that
	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(verticesBot),
		sizeof(texcoordsBot),
		texcoordsBot);

	GLuint  vPosition = glGetAttribLocation(program_1, "s_vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint	vTexcoord = glGetAttribLocation(program_1, "s_vTexcoord");
	glEnableVertexAttribArray(vTexcoord);
	glVertexAttribPointer(vTexcoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(verticesBot)));

	glUniform1i(glGetUniformLocation(program_1, "diffuseMap"), 2*7+5); // set the variable diffuseMap to 2 so that it uses texture2
	//glUniform1i(glGetUniformLocation(program_1, "normalMap"), 1*7+5); // set the variable normalMap to 1 so that it uses texture1
	glUniform1i(glGetUniformLocation(program_1, "normalMap"), 12); // set the variable normalMap to 1 so that it uses texture1

	glBindVertexArray(0);
	//*******************END Render Bottom Face****************

}

//Any time the windows is resized, this function is called.
//The parameters passed to it are the new dimensions of the window.
void changeViewport(int w, int h){
	glViewport(0,0,w,h);
}

//This function is called each time the window is redrawn.
void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	
	glDepthMask(false);
	//Draw Topfaces
	glUseProgram(program_0);

	glUniform3f(camPos, cam.position.x, cam.position.y, cam.position.z);
	cam.setCameraMatrix();

	glBindVertexArray(VAOs[TopFaces]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	

	//Draw Bottomface
	glUseProgram(program_1);
	glUniform3f(camPos, cam.position.x, cam.position.y, cam.position.z);

	cam.setCameraMatrix();
	glBindVertexArray(VAOs[BottomFace]);
	glDrawArrays(GL_TRIANGLES, 0, NumVerticesBot);

	glDepthMask(true);

	//draw object
	glUseProgram(program_object);
	glUniform3f(camPos, cam.position.x, cam.position.y, cam.position.z);


	cam.setCameraMatrix();
	m->Draw();

	GLint64 time;
	glGetInteger64v(GL_TIMESTAMP, &time);
	glUniform1f(time_unif, (float)time / 1000000000.0f);

	//Draw boxes
	

	if (draw_box)
	{
		glUseProgram(program_box);

		glUniform3f(camPos, cam.position.x, cam.position.y, cam.position.z);
		cam.setCameraMatrix();

		glBindVertexArray(VAOs[smallBox]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glutPostRedisplay();
	glFlush();
	glutSwapBuffers();
}



int main(int argc, char **argv){
	
	glutInit(&argc,argv);			 //Initialize GLUT

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_height, g_width);
	glutInitContextVersion(3, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Walking Dead, Floating Truck and The Hollow Box");


	glewExperimental = GL_TRUE;
	if (glewInit()) {
		cerr << "GLEW Init Error" << endl;
		exit(EXIT_FAILURE);
	}

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	loadTextures();

	init_topFaces();
	init_bottomFaces();

	init_object();

	if (draw_box)
	init_box();

	glutDisplayFunc(display);

	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);

	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);


	GLenum err = glewInit();		 //Very important! This initializes the entry points 
	if(GLEW_OK!=err){				 //in the OpenGL driver so we can call all the functions 
		cerr<<"GLEW error";			 //in the API.
		return 1;
	}

	glutMainLoop();
	
	return 0;
} 