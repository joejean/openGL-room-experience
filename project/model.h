#include "tiny_obj_loader.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>
#include <string>
#include <cmath>
//#include <array>
#include "stb_image.h"
//#include "texture.h"

template<int n>
struct array_t {
	GLuint buffer[n];
};

struct group_t {
	int start, count, material_id, texture_id;
};


class Model{
private:
	static const unsigned int NUM_BUFFERS = 4;
	std::vector<array_t<NUM_BUFFERS>> vbo;
	std::vector<GLuint> vao;
	std::vector<GLuint> textures;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<std::vector<group_t>> groups_by_material; 
	
	enum vboPositions{
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB
	};

	// lighting variable ids
	GLuint Ka_id;
	GLuint Kd_id;
	GLuint Ks_id;
	GLuint shininess_id;
	GLuint Kd_map_present_id;


	void operator=(const Model& model) {}
	Model(const Model& mesh) {}
	void normalize_coordinates(float maxCoordinateMagnitude);
	void change_orientation();
	void setup_buffers();
	GLint gen_texture(std::string& fileName, std::string& basepath);
	void setup_textures(std::string& basepath);
	void setup_groups_by_material_id();
	void get_lighting_ids(GLuint program);

public:
  Model(GLuint program, const char* filename, const char* basepath = NULL, bool changeOrientation = false, 
	    bool normalizeCoords = true, float maxCoordinateMagnitude = 0.7f);
  ~Model();
 
  void Draw();

};