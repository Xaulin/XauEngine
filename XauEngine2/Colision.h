#pragma once
#include "Model.h"
#include "GL\glm\glm.hpp"
#include <vector>

struct ColisionRect{
	glm::vec3 v[4];
};

class Colision{
public:
	friend Colision* generateColisionsFromModel(Model* model);

	~Colision();
	void add(ColisionRect rect);

private:
	std::vector<ColisionRect> triangleSquares;
};

