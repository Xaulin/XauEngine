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

void Scene::add(Object* element){
	objects.push_back(element);
}
void Scene::add(Object** elements, int size){
	for (int i = 0; i < size; ++i)
		objects.push_back(elements[i]);
}
void Scene::rel(unsigned slot){
	objects.erase(objects.begin() + slot);
}
void Scene::rel(unsigned beg, unsigned end){
	objects.erase(objects.begin() + beg, objects.begin() + end);
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
void Scene::draw(){
	currentShader->draw(objects);
}
IShader* Scene::get(){
	return currentShader;
}