#include "LoadShaders.h"

void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

GLuint LoadShaders(ShaderInfo shaderInfo)
{
	GLuint program = glCreateProgram(); // create program

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);	    // create a vertex shader object
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	// create a fragment shader object

	// load and compile vertex shader
	string vstext;
	const char* text = getShaderProgram(shaderInfo.vShaderFile, vstext);
	//GLint length = vstext.size();
	glShaderSource(vertexShader, 1, &text, NULL);
	glCompileShader(vertexShader);

	CheckShaderError(vertexShader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	// load and compile fragment shader
	string fstext;
	text = getShaderProgram(shaderInfo.fShaderFile, fstext);
	glShaderSource(fragmentShader, 1, &text, NULL);
	glCompileShader(fragmentShader);


	CheckShaderError(fragmentShader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	// attach the vertex and fragment shaders to the program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// link the objects for an executable program
	glLinkProgram(program);

	// return the program
	return program;
}

const char* getShaderProgram(const char *filePath, string &shadertext)
{
	fstream shaderFile(filePath, ios::in);

	if (shaderFile.is_open())
	{
		std::stringstream buffer;
		buffer << shaderFile.rdbuf();
		shadertext = buffer.str();
		buffer.clear();
	}
	shaderFile.close();

	return shadertext.c_str();
}


void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

