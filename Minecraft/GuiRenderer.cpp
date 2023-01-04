#include "GuiRenderer.h"

bool GuiRenderer::Init()
{
	crosshairVAO.Init(
		{
			{ CreateAttribute<0, glm::vec2, 0, 0>, crosshairVBO }
		}
	);

	crosshairProgram.Init(
		{
			{ GL_VERTEX_SHADER, "crosshair.vert" },
			{ GL_FRAGMENT_SHADER, "crosshair.frag" }
		},
		{
			{ 0, "vs_in_pos" }
		}
	);

	std::vector<glm::vec2> pos = { { -0.015, 0 }, { 0.015, 0 }, { 0, -0.015 * (16 / 9.f) }, { 0, 0.015 * (16 / 9.f) } };
	crosshairVBO.BufferData(pos);

	return true;
}

void GuiRenderer::Clean()
{
	crosshairVBO.Clean();
	crosshairProgram.Clean();
}

void GuiRenderer::Render()
{
	crosshairProgram.Use();
	crosshairVAO.Bind();
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, 4);
	crosshairProgram.Unuse();
}
