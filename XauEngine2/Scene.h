#pragma once
#include "GL\glm\gtx\transform.hpp"
#include "Model.h"
#include "Texture.h"
#include "IShader.h"
#include <vector>

class Scene{
public:
	Object* operator[](int slot);

	Scene();
	Scene(IShader* shader);
	~Scene();

	void add(Object* element);
	void add(Object** elements, int size);
	void rel(unsigned slot);
	void rel(unsigned beg, unsigned end);
	void del(unsigned slot);
	void del(unsigned beg, unsigned end);
	int size();
	void bind(IShader* shader);
	void draw();
	IShader* get();

private:
	std::vector<Object*> objects;
	IShader* currentShader;
};

