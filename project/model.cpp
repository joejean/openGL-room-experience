#include "model.h"

Model::Model(GLuint program, const char* filename, const char* basepath, bool changeOrientation, bool normalizeCoordinates, float maxCoordinateMagnitude){

	std::string err = tinyobj::LoadObj(shapes, materials, filename, basepath);
	if (!err.empty()) {std::cerr << err << std::endl; return;}

	if(changeOrientation){ // negate z coordinate in positions and normals
		change_orientation();
	}

	if(normalizeCoordinates){// also shifts each dimension
		normalize_coordinates(maxCoordinateMagnitude);
	}

	get_lighting_ids(program);
	setup_buffers();
	setup_textures(std::string(basepath));
	setup_groups_by_material_id(); //what is material id set to if there are no materials?

}

Model::~Model(){
	unsigned int num_shapes = shapes.size();
	for(unsigned int i = 0; i < num_shapes; ++i){
		glDeleteBuffers(NUM_BUFFERS, vbo[i].buffer);
		glDeleteVertexArrays(1, &vao[i]);
	}

	unsigned int num_textures = textures.size();
	for(unsigned int i = 0; i < num_textures; ++i){
			glDeleteTextures(1, &textures[i]);
	}
}


void Model::Draw(){ // check if material id is negative
	unsigned int num_shapes = shapes.size();
	for(unsigned int i = 0; i < num_shapes; ++i){
		glBindVertexArray(vao[i]);
		unsigned int num_groups = groups_by_material[i].size();
		for(unsigned int j = 0; j< num_groups; ++j){
			auto& g=groups_by_material[i][j];
			if(g.texture_id >= 0){
				glBindTexture(GL_TEXTURE_2D, g.texture_id);
				glUniform1i(Kd_map_present_id,1);
			}
			else{
				glUniform1i(Kd_map_present_id,0);
			}
			auto& m= materials[g.material_id];
			glUniform3fv(Ka_id, 1, m.ambient);
			glUniform3fv(Kd_id, 1, m.diffuse);
			glUniform3fv(Ks_id, 1, m.specular);
			glUniform1f(shininess_id, m.shininess);
			glDrawElements(GL_TRIANGLES, 3*g.count, GL_UNSIGNED_INT, (void *)(3*g.start * sizeof(GLuint)) );
		}
		
		glBindVertexArray(0);
	}
}

void Model::change_orientation(){
	unsigned int num_shapes = shapes.size();
	for(unsigned int counter = 0; counter < num_shapes; ++counter){
			auto& mesh = shapes[counter].mesh;
			auto& positions = mesh.positions;
			auto& normals = mesh.normals;

			unsigned int num_positions = positions.size();
			for(unsigned int j=2; j< num_positions; j+=3){
				positions[j]*=-1;
			}
			unsigned int num_normals = normals.size();
			for(unsigned int j=2; j< num_normals; j+=3){
				normals[j]*=-1;
			}
		}
}

void Model::normalize_coordinates(float maxCoordinateMagnitude){
	unsigned int num_shapes = shapes.size();
	float max[3], min[3];
		
	for(unsigned int j =0; j< 3; ++j){
		max[j] = min[j] = shapes[0].mesh.positions[j];
	}

	for(unsigned int counter = 0; counter < num_shapes; ++counter){

			auto& positions = shapes[counter].mesh.positions;
			unsigned int num_positions = positions.size();
			
			for(unsigned int j = 0; j < num_positions; ++j){
				float v = positions[j];
				if(v > max[j%3])
					max[j%3] = v;
				else if(v < min[j%3])
					min[j%3] = v;
			}
	}

	float shift[3], max_width=0;

	for(unsigned int j=0; j<3; ++j){
		shift[j] = -(max[j] + min[j])/2;
		if(max_width < max[j] - min[j]){
			max_width= max[j] - min[j];
		}
	}

	for(unsigned int counter = 0; counter < num_shapes; ++counter){
			auto& positions = shapes[counter].mesh.positions;
			unsigned int num_positions = positions.size();
			for(unsigned int j = 0; j < num_positions; ++j){
				positions[j] = (positions[j]+shift[j%3])*(maxCoordinateMagnitude/max_width);
			}

	}
}

void Model::setup_buffers(){
	unsigned int num_shapes = shapes.size();
	vao.resize(num_shapes);
	vbo.resize(num_shapes);

	for(unsigned int counter = 0; counter < num_shapes; ++counter){
		auto& m_vao = vao[counter];
		auto& buffer = vbo[counter].buffer;
		auto& mesh = shapes[counter].mesh;
		auto& positions = mesh.positions;
		auto& normals = mesh.normals;
		auto& texcoords = mesh.texcoords;
		auto& indices = mesh.indices;
		
		
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glGenBuffers(NUM_BUFFERS, buffer);
	
		if(positions.size() > 0){
			glBindBuffer(GL_ARRAY_BUFFER, buffer[POSITION_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if(texcoords.size() > 0){
			glBindBuffer(GL_ARRAY_BUFFER, buffer[TEXCOORD_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords[0]) * texcoords.size(), &texcoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if(normals.size() > 0){

			glBindBuffer(GL_ARRAY_BUFFER, buffer[NORMAL_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		if(indices.size() > 0){
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_VB]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
		}
		glBindVertexArray(0);
	}

}

void Model::setup_textures(std::string& basepath){
	unsigned int num_materials = materials.size();
	textures.resize(num_materials);
	for(unsigned int counter = 0; counter < num_materials; ++counter){
		textures[counter] = gen_texture(materials[counter].diffuse_texname, basepath);
	}
}

GLint Model::gen_texture(std::string& fileName, std::string& basepath){ 

	if(fileName=="") return -1;

	int width, height, numComponents;
	std::string fullFileName = basepath + fileName;
	unsigned char* data = stbi_load(fullFileName.c_str(), &width, &height, &numComponents, 4);

	if(data == NULL){
		std::cerr << "Unable to load texture: " << fullFileName << std::endl;
		return -1;
	}
	else{ // FLIP data since STB_IMAGE loads image with top left corner as origin and y axis running from top to bottom
		for(int i = 0; i < height/2; ++i){
			for(int j = 0; j < width; ++j){
				for(int k =0; k<4; ++k){
					int a = 4*(i*width + j)+k; 
					int b = 4*((height - 1 - i)*width + j)+k;
					unsigned char c = data[a];
					data[a] = data[b];
					data[b] = c;
				}
			}
		}
	}
	
	GLuint m_texture;
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
	return m_texture;

}

void Model::setup_groups_by_material_id(){	

	if(materials.size() <= 0){
		return;
	}
	
	unsigned int num_shapes = shapes.size();
	groups_by_material.resize(num_shapes);

	for(unsigned int counter = 0; counter< num_shapes; ++counter){

		auto& material_ids = shapes[counter].mesh.material_ids;
		unsigned int material_ids_size = material_ids.size();
		if(material_ids_size > 0){
			group_t g;
			g.start = 0;
			g.material_id = material_ids[0];
			if(g.material_id >= 0){
				g.texture_id = textures[g.material_id];
			}
			else{
				g.texture_id = -1;
			}
			
			int prev_mat_id = g.material_id; 
			for(unsigned int i = 1; i< material_ids_size; ++i){
				if(material_ids[i]!=prev_mat_id){
					g.count = i-g.start;
					groups_by_material[counter].push_back(g);
					g.start = i;
					g.material_id = material_ids[i];

					if(g.material_id >= 0){
							g.texture_id = textures[g.material_id];
					}
					else{
						g.texture_id = -1;
					}
				}
			}
			g.count = material_ids_size - g.start;
			groups_by_material[counter].push_back(g);
		}
	}
}
		

void Model::get_lighting_ids(GLuint program){
	Ka_id = glGetUniformLocation(program,"Ka");
	Kd_id = glGetUniformLocation(program,"Kd");
	Ks_id = glGetUniformLocation(program,"Ks");
	shininess_id = glGetUniformLocation(program, "shininess");
	Kd_map_present_id = glGetUniformLocation(program,"Kd_map_present");
}
	