#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl_gl3.h>

#include <iostream>
#include <sstream>

#include "MyApp.h"

void exitProgram()
{
	SDL_Quit();

	std::cout << "Press any key to exit..." << std::endl;
	std::cin.get();
}

int main( int argc, char* args[] )
{
	atexit( exitProgram );

	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		std::cout << "[SDL Init] An error has occured during the inicialization of SDL: " << SDL_GetError() << std::endl;
		return 1;
	}
			
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,         32);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,		1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          24);

	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

	SDL_Window *win = 0;
    win = SDL_CreateWindow( "Badcraft",
							100,
							100,
							1280,
							720,
							SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);


    if (win == 0)
	{
		std::cout << "[Window Init] Error during inicialization of window: " << SDL_GetError() << std::endl;
        return 1;
    }

	SDL_GLContext	context	= SDL_GL_CreateContext(win);
    if (context == 0)
	{
		std::cout << "[OGL context] Error during inicialization of SDL: " << SDL_GetError() << std::endl;
        return 1;
    }	

	SDL_GL_SetSwapInterval(1);

	GLenum error = glewInit();
	if ( error != GLEW_OK )
	{
		std::cout << "[GLEW] Error starting GLEW!" << std::endl;
		return 1;
	}

	int glVersion[2] = {-1, -1}; 
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); 
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); 
	std::cout << "Running OpenGL " << glVersion[0] << "." << glVersion[1] << std::endl;

	if ( glVersion[0] == -1 && glVersion[1] == -1 )
	{
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow( win );

		std::cout << "[OGL context] Couldn't create OGL Context! Perhaps one of the SDL_GL_SetAttribute(...) calls is wrong." << std::endl;

		return 1;
	}

	std::stringstream window_title;
	window_title << "Badcraft (OpenGL " << glVersion[0] << "." << glVersion[1] << ")";
	SDL_SetWindowTitle(win, window_title.str().c_str());

	ImGui_ImplSdlGL3_Init(win);

	{
		bool quit = false;
		SDL_Event ev;

		CMyApp app;
		if (!app.Init())
		{
			SDL_GL_DeleteContext(context);
			SDL_DestroyWindow(win);
			std::cout << "[app.Init] Error occured during inicialization of app!" << std::endl;
			return 1;
		}

		while (!quit)
		{
			while (SDL_PollEvent(&ev))
			{
				ImGui_ImplSdlGL3_ProcessEvent(&ev);
				bool is_mouse_captured = ImGui::GetIO().WantCaptureMouse;
				bool is_keyboard_captured = ImGui::GetIO().WantCaptureKeyboard;
				switch (ev.type)
				{
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					if (!is_keyboard_captured)
						app.KeyboardDown(ev.key);
					break;
				case SDL_KEYUP:
					if (!is_keyboard_captured)
						app.KeyboardUp(ev.key);
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (!is_mouse_captured)
						app.MouseDown(ev.button);
					break;
				case SDL_MOUSEBUTTONUP:
					if (!is_mouse_captured)
						app.MouseUp(ev.button);
					break;
				case SDL_MOUSEWHEEL:
					if (!is_mouse_captured)
						app.MouseWheel(ev.wheel);
					break;
				case SDL_MOUSEMOTION:
					if (!is_mouse_captured)
						app.MouseMove(ev.motion);
					break;
				case SDL_WINDOWEVENT:
					if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						app.Resize(ev.window.data1, ev.window.data2);
					}
					break;
				}

			}
			ImGui_ImplSdlGL3_NewFrame(win);

			app.Update();
			app.Render();
			ImGui::Render();

			SDL_GL_SwapWindow(win);
		}

		app.Clean();
	}

	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow( win );

	return 0;
}