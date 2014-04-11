#pragma once
#include <assert.h>
#include <vector>
#include "GL\glm\gtx\transform.hpp"
#include "Model.h"
#include "Texture.h"
#include "IShader.h"

class Scene{
public:
	Object* operator[](int slot);

	Scene();
	Scene(IShader* shader);
	~Scene();

	void add(Object* element);
	void del(unsigned slot);
	void bind(IShader* shader);
	void draw();
	IShader* get();

private:
	std::vector<Object*> objects;
	IShader* currentShader;
};

