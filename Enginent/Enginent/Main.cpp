

// OpenGL headers
#include <GL/glew.h>
/*#include <GL/glu.h>
#include <GL/gl.h>*/
#include "pch.h"

// SDL headers
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_ttf.h>

#include <iostream>
#include <string>
#include "GLRenderer.h"
#include "SquareMeshVbo.h"
#include "TimeSystem.h"
#include "MouseInput.h"

#include "Game.h"

using namespace std;
bool quit;
const GLint WIDTH = 1280, HEIGHT = 720;

SDL_Window* window;
SDL_GLContext glContext;
SDL_Event sdlEvent;

Game * game = nullptr;
MouseInput* mouseInput = nullptr;
bool mouseDrag = false;

int main(int argc, char *argv[])
{
	quit = false;

	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Initialize video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// Display error message
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		return false;
	}
	else
	{
		if (TTF_Init() == -1) {
			std::cout << "TTF_Init: " <<  TTF_GetError() << std::endl;
			return false;
		}
		// Create window
		window = SDL_CreateWindow("Underground", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			// Display error message
			cout << "Window could not be created! SDL_Error" << SDL_GetError() << endl;
			return false;
		}
		else
		{
			//Fullscreen enable here!
			//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			// Create OpenGL context
			glContext = SDL_GL_CreateContext(window);

			if (glContext == NULL)
			{
				// Display error message
				cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << endl;
				return false;
			}
			else
			{
				// Initialize glew
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK)
				{
					cout << "Error initializing GLEW! " << glewGetErrorString(glewError) << endl;
					return false;
				}
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << endl;
				}
				game = Game::GetInstance();
				mouseInput = MouseInput::GetInstance();
				game->Init(WIDTH, HEIGHT);
				mouseInput->Init(WIDTH, HEIGHT);
			}
		}
	}

	TimeSystem::instance()->FrameInit();

	// Game loop
	while (!quit && !game->GetInstance()->getQuitState())
	{
		while (SDL_PollEvent(&sdlEvent) != 0)
		{
			// Esc button is pressed
			if (sdlEvent.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (sdlEvent.type == SDL_KEYDOWN) {
				// temp debug
				game->HandleKey(sdlEvent.key.keysym.sym);
			}
			else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
				if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
					mouseInput->UpdateMouseInput(RightClick, sdlEvent.button.x, sdlEvent.button.y);
				}
				else if (sdlEvent.button.button == SDL_BUTTON_LEFT)
				{
					mouseInput->UpdateMouseInput(LeftClick, sdlEvent.button.x, sdlEvent.button.y);
				}
			}
			else if (sdlEvent.type == SDL_MOUSEBUTTONUP) {
				if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
					mouseInput->UpdateMouseInput(RightRelease, sdlEvent.button.x, sdlEvent.button.y);
				}
				else if (sdlEvent.button.button == SDL_BUTTON_LEFT)
				{
					mouseInput->UpdateMouseInput(LeftRelease, sdlEvent.button.x, sdlEvent.button.y);
				}
			}
			else if (sdlEvent.type == SDL_MOUSEWHEEL) {
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				mouseInput->UpdateMouseInput(Scroll, mouseX, mouseY, sdlEvent.wheel.y);
			}
			else if (sdlEvent.type == SDL_MOUSEMOTION) {
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				mouseInput->UpdateMouseInput(Hover, mouseX, mouseY);
			}
		}

		TimeSystem::instance()->FrameStart();
		game->Update();
		game->Render();

		SDL_GL_SwapWindow(window);
		TimeSystem::instance()->FrameEnd();
	}

	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();

	// delete game
	delete game;

	return 0;
}


