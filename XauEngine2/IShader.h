#pragma once
#include "Model.h"
#include "Texture.h"
#include "Object.h"
#include <vector>

enum ObjectOptions{
	None = 0,
	NoDepthTest = 1,
	NoShadow = 2,
	NoCullFacing = 4,
	CullFacing = 8,
	NoLight = 16,
	SimpleShader = 32,
	StickToCamera = 64,
	SkyBox = 128 + 64 + 32 + 16 + 2 + 1,
	ShadowToTexture = 256,
	NoVisible = 512,
};

class IShader
{
public:
	virtual ~IShader() = 0;
	virtual void draw(std::vector<Object*>& objects) = 0;
};

