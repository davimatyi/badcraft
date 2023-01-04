#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "Includes/ProgramObject.h"
#include "Includes/VertexArrayObject.h"
#include "Includes/BufferObject.h"
#include "Includes/TextureObject.h"
#include "Includes/gCamera.h"

class SkyboxRenderer
{
public:
	bool Init(gCamera*);
	void Clean();

	void Render();

protected:
	ProgramObject skyboxProgram;
	VertexArrayObject skyboxVAO;
	ArrayBuffer skyboxVBO;
	IndexBuffer skyboxIB;

	gCamera* camera;

	GLuint skyboxTexture;
};
