#include "SkyboxRenderer.h"
#include "Includes/GLUtils.hpp"

bool SkyboxRenderer::Init(gCamera* camera)
{
	this->camera = camera;

	skyboxVBO.BufferData(
		std::vector<glm::vec3>{
		glm::vec3(-1, -1, -1),
			glm::vec3(1, -1, -1),
			glm::vec3(1, 1, -1),
			glm::vec3(-1, 1, -1),
			glm::vec3(-1, -1, 1),
			glm::vec3(1, -1, 1),
			glm::vec3(1, 1, 1),
			glm::vec3(-1, 1, 1),
	}
	);

	skyboxIB.BufferData(
		std::vector<int>{
		0, 1, 2,
			2, 3, 0,
			4, 6, 5,
			6, 4, 7,
			0, 3, 4,
			4, 3, 7,
			1, 5, 2,
			5, 6, 2,
			1, 0, 4,
			1, 4, 5,
			3, 2, 6,
			3, 6, 7,
	}
	);

	skyboxVAO.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, skyboxVBO },
		}, skyboxIB
		);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glGenTextures(1, &skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	TextureFromFileAttach("assets/xpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	TextureFromFileAttach("assets/xneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	TextureFromFileAttach("assets/ypos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	TextureFromFileAttach("assets/yneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	TextureFromFileAttach("assets/zpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	TextureFromFileAttach("assets/zneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	skyboxProgram.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },
		}
		);

	return true;
}

void SkyboxRenderer::Clean()
{
	glDeleteTextures(1, &skyboxTexture);
	skyboxVBO.Clean();
	skyboxIB.Clean();
	skyboxProgram.Clean();
}

void SkyboxRenderer::Render()
{
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	glDepthFunc(GL_LEQUAL);

	skyboxVAO.Bind();
	skyboxProgram.Use();
	skyboxProgram.SetUniform("MVP", camera->GetViewProj() * glm::translate(camera->GetEye()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glUniform1i(skyboxProgram.GetLocation("skyboxTexture"), 0);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	skyboxProgram.Unuse();

	glDepthFunc(prevDepthFnc);
}