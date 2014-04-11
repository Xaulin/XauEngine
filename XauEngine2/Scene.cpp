#include "Scene.h"

Object* Scene::operator[](int slot){
	return objects[slot];
}

Scene::Scene() : currentShader(0){
}
Scene::~Scene(){
	delShader();
}

void Scene::addObject(Object* element){
	objects.push_back(element);
}
void Scene::delObject(unsigned slot){
	objects.erase(objects.begin() + slot);
}

void Scene::bindShader(Shader* shader){
	currentShader = shader;
}
void Scene::delShader(){
	delete currentShader;
	currentShader = 0;
}

void Scene::draw(){
	currentShader->draw(objects);
}