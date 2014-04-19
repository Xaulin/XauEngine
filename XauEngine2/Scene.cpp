#include "Scene.h"

Object* Scene::operator[](int slot){
	return objects[slot];
}

Scene::Scene() : currentShader(0){
}
Scene::Scene(IShader* shader){
	currentShader = shader;
}
Scene::~Scene(){
	delete currentShader;
	/*for (auto& o : objects){
		o
		delete o;
	}*/
}
#include <Windows.h>
int Scene::add(Object* element){
	objects.push_back(element);
	return objects.size()-1;
}
int Scene::add(Object** elements, int size){
	for (int i = 0; i < size; ++i)
		objects.push_back(elements[i]);
	return objects.size()-1;
}
void Scene::rel(unsigned slot){
	objects.erase(objects.begin() + slot);
}
void Scene::rel(unsigned beg, unsigned end){
	
	/*char buf[64];
	sprintf_s(buf, "%d %d", beg, end);
	MessageBoxA(0, buf, 0, 0);*/
	objects.erase(objects.begin()+beg, objects.begin() + end);
}
void Scene::del(unsigned slot){
	delete objects[slot];
	rel(slot);
}
void Scene::del(unsigned beg, unsigned end){
	for (; beg < end; ++beg)
		delete objects[beg];
	rel(beg, end);
}
int Scene::size(){
	return objects.size();
}
void Scene::bind(IShader* shader){
	currentShader = shader;
}

extern CRITICAL_SECTION section;

void Scene::draw(){
	EnterCriticalSection(&section);
	currentShader->draw(objects);
	LeaveCriticalSection(&section);
}
IShader* Scene::get(){
	return currentShader;
}