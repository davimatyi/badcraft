#pragma once

#include <memory>

#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

#include "includes/ObjParser_OGL3.h"
#include "SkyboxRenderer.h"
#include "ChunkRenderer.h"
#include "GuiRenderer.h"
#include "SelectionHandler.h"
#include "WorldGenerator.h"
#include <mutex>

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

protected:
	ProgramObject		m_program;			

	gCamera				m_camera;

	SkyboxRenderer		skybox;
	ChunkRenderer		chunkRenderer;
	GuiRenderer			guiRenderer;

	BlockData           blockData;
	SelectionHandler    selection;
	WorldGenerator		worldGenerator;

	std::unordered_map<chunkID, ChunkData*> chunks;
	std::mutex chunks_access;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	void InitShaders();

	float fps_timer;
	float frame_count;
	int fps;

	block blockInHand = 5;

	bool generated = false;
};

