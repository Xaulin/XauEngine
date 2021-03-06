#pragma once
#include <cstdio>
#include <string.h>
#include "GL\glew.h"
#include "GL\glm\glm.hpp"

class Model{
public:
	friend Model* loadModel(char* filepath);
	friend Model* loadHeightMap(float x, float y, float z, char* path);
	friend Model* createPlane(unsigned x, unsigned z, float a);
	~Model();
	GLuint* VBOs;
	unsigned vcount;
};