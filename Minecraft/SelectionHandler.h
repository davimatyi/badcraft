#pragma once

#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "Includes/gCamera.h"
#include "BlockData.h"


class SelectionHandler
{
public:
	bool Init(gCamera*, BlockData*);
	void Clean();

	block GetSelectedBlock();
	glm::ivec3 GetBlockPos();
	glm::ivec3 GetPlacePos();

protected:
	float reach = 5;
	float increment = 0.05f;

	gCamera* camera;
	BlockData* blockData;
};