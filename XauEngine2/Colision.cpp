#include "Colision.h"

Colision* generateColisionsFromModel(Model* model){
/*	glm::vec3* v = (glm::vec3*)glMapBuffer(model->VBOs[1], GL_READ_ONLY);



	glUnmapBuffer(model->VBOs[1]);*/
	return 0;
}

Colision::~Colision(){
}

void Colision::add(ColisionRect rect){
	triangleSquares.push_back(rect);
}