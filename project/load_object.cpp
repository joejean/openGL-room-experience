#include "includes.h"
//#include "transform.h"
#include "LoadShaders.h"


using namespace std;
#define BUFFER_OFFSET(offset) ( (void *) offset )

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 600;

GLuint time_unif;

Model *m; 

GLuint program_object;

void init_object(void){

	ShaderInfo shader_object = { GL_VERTEX_SHADER, "basicShader.vs", GL_FRAGMENT_SHADER, "basicShader.fs" };
	program_object = LoadShaders(shader_object);

	//m = new Model(program,"Objects/pear/pear.obj", "Objects/pear/", false, true, 1);
	//m = new Model(program,"Objects/uh60/uh60.obj", "Objects/uh60/", false, true, 1);
	
	//m = new Model(program,"Objects/Starship/Starship.obj", "Objects/Starship/", false, true, 1);
	//m = new Model(program,"Objects/Rifle/Rifle.obj", "Objects/Rifle/", false, true, 1);
	//m = new Model(program,"Objects/dpv/dpv.obj", "Objects/dpv/", false, true, 1);
	//m = new Model(program,"Objects/ferrari_599gtb/ferrari_599gtb.obj", "Objects/ferrari_599gtb/", false, true, 1);
	//m = new Model(program, "Objects/zombie/zombie.obj", "Objects/zombie/", false, true, 1);
	m = new Model(program_object, "Objects/Swan/Swan.obj", "Objects/Swan/", false, true, 1);
	//m = new Model(program, "Objects/Dog/ZombiDog.obj", "Objects/Dog/", false, true, 1);
	//m = new Model(program, "Objects/Nurseknife/Nurse.obj", "Objects/Nurseknife/", false, true,10);
	//m = new Model(program, "Objects/Policeman/Policeman.obj", "Objects/Policeman/", false, true,1);


	time_unif = glGetUniformLocation(program_object,"time");

}

void display_object(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m->Draw();
			
	GLint64 time;
	glGetInteger64v(GL_TIMESTAMP, &time);
	glUniform1f(time_unif,(float)time/1000000000.0f);

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}
