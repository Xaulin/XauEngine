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
	for (auto& o : objects)
		delete o;
}

void Scene::add(Object* element){
	objects.push_back(element);
}
void Scene::del(unsigned slot){
	delete objects[slot];
	objects.erase(objects.begin() + slot);
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