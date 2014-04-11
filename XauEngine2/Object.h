#pragma once
#include "Texture.h"
#include "Model.h"
#include "GL\glm\gtx\transform.hpp"

class Object{
public:
	Object();
	Object(Model* model, Texture* texture, int options = 0);
	~Object();

	void setModel(Model* model);
	void setTexture(Texture* texture);

	void move(glm::vec3& vec);
	void rotate(glm::vec4& vec);
	void scale(glm::vec3& vec);

	void moveNext(glm::vec3& vec);
	void rotateNext(glm::vec4& vec);
	void scaleNext(glm::vec3& vec);

	Model* model;
	Texture* texture;
	glm::mat4 mat;
	int options;
};

