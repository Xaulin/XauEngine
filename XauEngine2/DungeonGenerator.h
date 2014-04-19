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

enum DungeonObjectFlags{
	End = 1
};

class DungeonGenerator{
public:
	DungeonGenerator();
	~DungeonGenerator();

	void defineObject(Object* object, glm::vec3* dirs, int size, int flags = None, int randomRate = 1);
	void generate(int count, float distance);
	void applyObjects(Scene* scene);
	void clear();

private:
	std::vector<DungeonObject> objects;
	std::vector<DungeonObject> endObjects;
	std::vector<DungeonObject*> defObjects;
	std::vector<DungeonObject*> defEndObjects;

	Scene* lastScene;
	int firstSceneElement;
};

