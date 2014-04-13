#pragma once
#include "Object.h"
#include "Scene.h"
#include <vector>
#include <Windows.h>

class DungeonObject{
public:
	Object* object;
	glm::vec3* dirs;
	int dirsSize;
	glm::vec3 pos;
};

class DungeonGenerator{
public:
	DungeonGenerator();
	~DungeonGenerator();

	void defineObject(Object* object, glm::vec3* dirs, int size);
	void generate(int count);
	void applyObjects(Scene* scene);
	void clean();

private:
	std::vector<DungeonObject> objects;
	std::vector<DungeonObject> defObjects;

};

