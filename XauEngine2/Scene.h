#pragma once
#include <assert.h>
#include <vector>
#include "GL\glm\gtx\transform.hpp"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class Scene{
public:
	Object* operator[](int slot);

	Scene();
	~Scene();

	void addObject(Object* element);
	void delObject(unsigned slot);

	void bindShader(Shader* shader);
	void delShader();

	void draw();

private:
	std::vector<Object*> objects;
	Shader* currentShader;
};

