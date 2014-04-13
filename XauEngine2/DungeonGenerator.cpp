#include "DungeonGenerator.h"


DungeonGenerator::DungeonGenerator(){
}


DungeonGenerator::~DungeonGenerator(){
	clean();
}

void DungeonGenerator::defineObject(Object* object, glm::vec3* dirs, int size){
	defObjects.push_back({ object, dirs, size });
}
void DungeonGenerator::generate(int count){
	clean();
	srand(GetCurrentTime());

	//firstElement
	objects.push_back(defObjects[rand() % defObjects.size()]);
	objects[0].pos = glm::vec3(0, 0, 0);

	for (int i = 0; i < count; ++i){
		bool valid = true;
		int randomObjectIndex = rand() % objects.size();
		int randomDir = rand() % objects[randomObjectIndex].dirsSize;
		int t = rand() % defObjects.size();
		glm::vec3 pos = objects[randomObjectIndex].pos + objects[randomObjectIndex].dirs[randomDir];
		for (auto o : objects) if (o.pos == pos){
			valid = false;
			break;
		}
		if (valid) objects.push_back({
			&(*(new Object) = *defObjects[t].object),
			defObjects[t].dirs,
			defObjects[t].dirsSize,
			pos
		});
	}
}
void DungeonGenerator::applyObjects(Scene* scene){
	for (auto o : objects){
		o.object->move(o.pos * 16.f);
		scene->add(o.object);
	}
}
void DungeonGenerator::clean(){

}