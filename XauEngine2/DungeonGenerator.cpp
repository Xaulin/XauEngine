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
	srand(666);

	//firstElement
	objects.push_back(defObjects[rand() % defObjects.size()]);
	objects[0].pos = glm::vec3(0, 0, 0);


	//****************************** WELCOME TO HELL \m / ******************************

	//next
	for (int i = 0; i < count; ++i){
		bool valid = true;
		int randomObjectIndex = rand() % objects.size();
		int randomDir = rand() % objects[randomObjectIndex].dirsSize;
		int t = rand() % defObjects.size();
		defObjects[t].pos = objects[randomObjectIndex].pos +
			objects[randomObjectIndex].dirs[randomDir];
		for (auto o : objects) if (o.pos == defObjects[t].pos){
			valid = false;
			break;
		}
		if (valid){
			for (int j = 0; j < defObjects[t].dirsSize; ++j){
				if (defObjects[t].dirs[j] == -objects[randomObjectIndex].dirs[randomDir]){
					bool valid2 = true;
					for (auto o : objects){
						for (int k = 0; k < o.dirsSize; ++k)
							if (o.pos + o.dirs[k] == defObjects[t].pos){
								valid2 = false;
								for (int l = 0; l < defObjects[t].dirsSize; ++l)
									if (o.dirs[k] == -defObjects[t].dirs[l]){
										valid2 = true;
										goto BreakFirstLoop;
									}
									goto BreakMainLoop;
							}
					BreakFirstLoop:;
						for (int k = 0; k < defObjects[t].dirsSize; ++k)
							if (defObjects[t].pos + defObjects[t].dirs[k] == o.pos){
								valid2 = false;
								for (int l = 0; l < o.dirsSize; ++l)
									if (o.dirs[l] == -defObjects[t].dirs[k]){
										valid2 = true;
										goto BreakSecondLoop;
									}
									goto BreakMainLoop;
							}
					BreakSecondLoop:;
					}
					objects.push_back({
						&(*(new Object) = *defObjects[t].object),
						defObjects[t].dirs,
						defObjects[t].dirsSize,
						defObjects[t].pos
					});
					break;
				}
			BreakMainLoop:;
			}
		}
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