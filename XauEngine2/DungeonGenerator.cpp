#include "DungeonGenerator.h"


DungeonGenerator::DungeonGenerator(){
}

DungeonGenerator::~DungeonGenerator(){
	clear();
}

void DungeonGenerator::defineObject(Object* object, glm::vec3* dirs, int size, int flags, int randomRate){
	auto obj = new DungeonObject;
	obj->object = object;
	obj->dirs = dirs;
	obj->dirsSize = size;

	if (flags & End)
		defEndObjects.push_back(obj);
	else for (int i = 0; i < randomRate; ++i)
		defObjects.push_back(obj);
}
void DungeonGenerator::generate(int count, float distance){
	srand(GetCurrentTime());

	//firstElement
	if (!objects.size()){
		objects.push_back({
			&(*(new Object) = *defObjects[0]->object),
			defObjects[0]->dirs,
			defObjects[0]->dirsSize,
			defObjects[0]->pos
		});

		objects[0].pos = glm::vec3(0, 0, 0);
	}
	//****************************** WELCOME TO HELL \m / ******************************

	//next
	for (int i = 0; i < count; ++i){
		bool valid = true;
		int randomObjectIndex = rand() % objects.size();
		int randomDir = rand() % objects[randomObjectIndex].dirsSize;
		int t = rand() % defObjects.size();
		defObjects[t]->pos = objects[randomObjectIndex].pos +
			objects[randomObjectIndex].dirs[randomDir];
		if (glm::length(defObjects[t]->pos) < distance){
			for (auto o : objects) if (o.pos == defObjects[t]->pos){
				valid = false;
				break;
			}
			if (valid){
				for (int j = 0; j < defObjects[t]->dirsSize; ++j){
					if (defObjects[t]->dirs[j] == -objects[randomObjectIndex].dirs[randomDir]){
						bool valid2 = true;

						for (auto o : objects){
							for (int k = 0; k < o.dirsSize; ++k)
								if (o.pos + o.dirs[k] == defObjects[t]->pos){
									valid2 = false;
									for (int l = 0; l < defObjects[t]->dirsSize; ++l)
										if (o.dirs[k] == -defObjects[t]->dirs[l]){
											valid2 = true;
											goto BreakFirstLoop;
										}
									goto BreakMainLoop;
								}
						BreakFirstLoop:;
							for (int k = 0; k < defObjects[t]->dirsSize; ++k)
								if (defObjects[t]->pos + defObjects[t]->dirs[k] == o.pos){
									valid2 = false;
									for (int l = 0; l < o.dirsSize; ++l)
										if (o.dirs[l] == -defObjects[t]->dirs[k]){
											valid2 = true;
											goto BreakSecondLoop;
										}
									goto BreakMainLoop;
								}
						BreakSecondLoop:;
						}
						objects.push_back({
							&(*(new Object) = *defObjects[t]->object),
							defObjects[t]->dirs,
							defObjects[t]->dirsSize,
							defObjects[t]->pos
						});
						break;
					}
				BreakMainLoop:;
				}
			}
		}
	}

	for (auto o : objects)
		for (int j = 0; j < o.dirsSize; ++j){
			bool valid = true;
			for (auto o2 : objects)
				if (o.pos + o.dirs[j] == o2.pos){
					valid = false;
					break;
				}
			if (valid){
				for (auto o3 : defEndObjects)
					if (o3->dirs[0] == -o.dirs[j]){
						DungeonObject obj;
						obj.object = &(*(new Object) = *o3->object);
						obj.pos = o.pos + o.dirs[j];
						endObjects.push_back(obj);
					}
			}
		}

}
void DungeonGenerator::applyObjects(Scene* scene){
	if (objects.size() > 0){
		lastScene = scene;
		firstSceneElement = scene->add(objects[0].object);

		for (int i = 1; i < objects.size(); ++i){
			objects[i].object->move(glm::vec3(objects[i].pos.x * 16.f, objects[i].pos.y * 10.f,
				objects[i].pos.z*16.f));
			scene->add(objects[i].object);
		}
		for (auto o : endObjects){
			o.object->move(glm::vec3(o.pos.x * 16.f, o.pos.y * 10.f, o.pos.z*16.f));
			scene->add(o.object);
		}
	}
}
void DungeonGenerator::clear(){
	int totalSize = objects.size() + endObjects.size();
	if (totalSize > 0){
		lastScene->rel(firstSceneElement, firstSceneElement+totalSize);

		for (auto o : objects)
			delete o.object;
		for (auto o : endObjects)
			delete o.object;

		objects.clear();
		endObjects.clear();
	}
}