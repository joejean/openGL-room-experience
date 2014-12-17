#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

#include "GL/glew.h"
#include "GL/freeglut.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtx/transform.hpp>


using namespace std;

class Mesh {
private:
	
	
public:
	
	GLuint num_vertices;
	GLuint num_faces;
	vector<GLfloat> vertices;
	vector<GLuint> faces;
	vector<GLfloat> colors;
	vector<GLfloat> normals;
	
	
	Mesh();
	int Load(char* filename);
	void normalize(float max_mag);
	void computeNormals();

};