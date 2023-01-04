#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>

std::string floatp2(float f) {
	return std::to_string(f).substr(0, std::to_string(f).find(".") + 3);
}


CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(0, 34.85f, 0), glm::vec3(2, 34.85f, 0), glm::vec3(0, 1, 0));
}

CMyApp::~CMyApp(void)
{
	std::cout << "App destructor called.\n";
}

bool CMyApp::Init()
{
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	InitShaders();

	m_camera.SetProj(45.0f, 16.f / 9.f, 0.01f, 1000.0f);

	skybox.Init(&m_camera);


	chunkRenderer.Init(&chunks, &chunks_access);
	guiRenderer.Init();

	blockData.Init(&chunks, &chunks_access);
	selection.Init(&m_camera, &blockData);
	worldGenerator.Init(&blockData, 0, &chunks_access);

	

	fps_timer = 0;
	frame_count = 0;
	fps = 0;

	return true;
}

void CMyApp::InitShaders()
{
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
		});

	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_norm" },
		{ 2, "vs_in_tex" },
		});

	m_program.LinkProgram();
}


void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();

	if (!generated) {
		std::vector<chunkID> chunksToGenerate;

		for (int i = -5; i <= 5; ++i)
			for (int j = -5; j <= 5; ++j)
				chunksToGenerate.push_back({ i,j });

		worldGenerator.GenerateChunks(chunksToGenerate);
		generated = true;
	}
}

void CMyApp::Render()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	last_time = SDL_GetTicks();
	fps_timer += delta_time;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProj = m_camera.GetViewProj();

	m_program.Use();

	

	m_program.Unuse();

	skybox.Render();
	
	chunks_access.lock();
	for (const auto& chunk : chunks) {
		chunkRenderer.Render(chunk.second, viewProj);
	}
	chunks_access.unlock();

	guiRenderer.Render();


	if (fps_timer >= 1) {
		fps = frame_count;
		frame_count = 0;
		fps_timer = 0;
	}

	if(ImGui::Begin("kill me pls")) {
		std::string pos = "XYZ: " + floatp2(m_camera.GetEye().x) + " / "
			+ floatp2(m_camera.GetEye().y) + " / " + floatp2(m_camera.GetEye().z);
		ImGui::Text(pos.c_str());
		std::string frm = std::to_string(fps) + " fps";
		ImGui::Text(frm.c_str());
		ImGui::Text(("looking at block: " + blockData.blockNames[selection.GetSelectedBlock()]).c_str());
		ImGui::Text(("block in hand: " + blockData.blockNames[blockInHand]).c_str());
	}
	ImGui::End();

	++frame_count;
}

void CMyApp::Clean()
{
	skybox.Clean();
	chunkRenderer.Clean();
	guiRenderer.Clean();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
	if (!m_camera.IsMouseCaptured()) return;
	if (mouse.button == SDL_BUTTON_LEFT) {
		if (selection.GetSelectedBlock() != 0) {
			blockData.DestroyBlock(selection.GetBlockPos());
		}
	}
	else if (mouse.button == SDL_BUTTON_RIGHT) {
		if (selection.GetSelectedBlock() != 0) {
			blockData.PlaceBlock(selection.GetPlacePos(), blockInHand);
		}
	}
	else if (mouse.button == SDL_BUTTON_MIDDLE) {
		block b = selection.GetSelectedBlock();
		if (b != 0) blockInHand = b;
	}
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);
}
