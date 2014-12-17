#include "Mesh.h"

Mesh::Mesh(){
	num_vertices = num_faces = 0;
	srand ((unsigned int)time(NULL));
}


int Mesh::Load(char *filename){
	
	char c,d;
	char l[100];
	//float default_color[3] = {(float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX };
	float default_color[3] = {0,0.5,1};
	GLfloat x;
	GLuint y;

	ifstream fin(filename);
	if(!fin.is_open()) return -1;

	fin >> c;
	fin.putback(c);
	if('0' <= c && c <= '9'){
		fin>>num_vertices;
		fin>>num_faces;
	}
	
	while(fin>>c){
		fin.get(d);
		if(d==' '){
			if(c=='v'){	
				for(int i=0; i<3; ++i){
					fin >> x;
					vertices.push_back(x);
					switch(i){
					case 0: x = 0.2f + (float)rand()/RAND_MAX; break;
					case 1: x = 0.2f + (float)rand()/RAND_MAX; break;
					case 2: x = 0.2f + (float)rand()/RAND_MAX; break;
					}
					//colors.push_back(x);
					colors.push_back(default_color[i]);
				}
			}
			else if(c=='f'){
				for(int i=0; i<3; ++i){
					fin >> y;
					faces.push_back(y-1);
				}

			}
		}
		else{
			fin.getline(l, 80);
		}
	}

	//cout<<vertices.size()<<" "<<faces.size()<<endl;
	num_vertices = vertices.size()/3;
	num_faces = faces.size()/3;
	return 0;
}

void Mesh::computeNormals(){
	vector<glm::vec3> vertices_vec3;
	vector<glm::vec3> normals_vec3;

	for(int i = 0; i < num_vertices; ++i){
		vertices_vec3.push_back(glm::vec3(vertices[3*i],vertices[3*i+1],vertices[3*i+2]));
		normals_vec3.push_back(glm::vec3(0.0, 0.0, 0.0));
	}
	
	for(int i = 0; i < num_faces; ++i){
		glm::vec3 v[3];
		for(int j = 0; j < 3; ++j){
			v[j] = vertices_vec3[faces[3*i+j]];
		}
		glm::vec3 n = glm::normalize(glm::cross(v[1]-v[0], v[2]-v[0]));
		for (int j = 0; j < 3; ++j){
			normals_vec3[faces[3*i+j]]+=n;
		}
	}
	
	for(int i = 0; i < num_vertices; ++i){
		normals_vec3[i] = glm::normalize(normals_vec3[i]);
	
		for(int j = 0; j< 3; ++j){
			normals.push_back(normals_vec3[i][j]);
		}
		
	}
	/**/
}

void Mesh::normalize(float max_mag){
	
	float min[3];
	float max[3];
	
	for(int j=0; j<3; ++j){
		min[j] = max[j] = vertices[j];
	}

	for(int i = 0; i<num_vertices;  ++i){
		for(int j = 0; j < 3; ++j){
			float x = vertices[3*i+j];
			if(x < min[j]){
				min[j] = x;
			}
			else if(x > max[j]){
				max[j] = x;
			}
		}
	}

	float mx=0;

	for(int j = 0; j <3; ++j){
		float x =  max[j] - min[j];
		mx = (x>mx)?x:mx;
	}

	for(int i=0; i< num_vertices; ++i){
		for(int j=0; j<3; ++j){
			vertices[3*i+j] = (2*vertices[3*i+j] - max[j] - min[j])*max_mag/mx ;
		}
	}

}