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

class GuiRenderer
{
public:
	bool Init();
	void Clean();

	void Render();

protected:
	ProgramObject crosshairProgram;
	VertexArrayObject crosshairVAO;
	ArrayBuffer crosshairVBO;
};