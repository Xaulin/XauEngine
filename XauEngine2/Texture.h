#pragma once
#include "GL\glew.h"
#include "GL\glm\glm.hpp"
#include <cstdio>
#include <string.h>

class Texture
{
public:
	friend Texture* loadTexture(char* filepath);
	~Texture();
	GLuint id;
};

