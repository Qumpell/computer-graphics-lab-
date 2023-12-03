#include "Texture.h"

#include <fstream> 
#include <iostream>
#include <iterator>
#include <vector>
#include "SOIL/SOIL.h"
#include <string>

typedef unsigned char byte;

GLuint Core::LoadTexture( const char * filepath )
{
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int w, h;
	unsigned char* image = SOIL_load_image(filepath, &w, &h, 0, SOIL_LOAD_RGBA);
	

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	return id;
}



GLuint Core::LoadCubemap(const std::vector<std::string>& faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height;
	unsigned char* data;

	for (GLuint i = 0; i < faces.size(); i++)
	{
		data = SOIL_load_image(("textures/skybox/" + faces[i]).c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			SOIL_free_image_data(data);
		}
		else
		{
			std::cerr << "Failed to load texture: " << faces[i] << std::endl;
			SOIL_free_image_data(data);
			glDeleteTextures(1, &textureID);
			return 0;
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


void Core::SetActiveTexture(GLuint textureID, const char * shaderVariableName, GLuint programID, int textureUnit)
{
	glUniform1i(glGetUniformLocation(programID, shaderVariableName), textureUnit);
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}
