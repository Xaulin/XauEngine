#include "Object.h"

Object::Object() : 
model(0), texture(0),
mat(glm::mat4()),
options(0){}

Object::Object(Model* _model, Texture* _texture, int _options) : 
model(_model),
texture(_texture),
mat(glm::mat4()),
options(_options){}

Object::~Object(){
	delete model;
	delete texture;
}

void Object::setModel(Model* model){
	this->model = model;
}
void Object::setTexture(Texture* texture){
	this->texture = texture;
}

void Object::move(glm::vec3& vec){
	mat = glm::translate(vec);
}
void Object::rotate(glm::vec4& vec){
	mat = glm::rotate(vec.w, glm::vec3(vec));
}
void Object::scale(glm::vec3& vec){
	mat = glm::scale(vec);
}
void Object::moveNext(glm::vec3& vec){
	mat = glm::translate(mat, vec);
}
void Object::rotateNext(glm::vec4& vec){
	mat = glm::rotate(mat, vec.w, glm::vec3(vec));
}
void Object::scaleNext(glm::vec3& vec){
	mat = glm::scale(mat, vec);
}