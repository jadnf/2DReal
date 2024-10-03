#include "Renderer.h"
#include "Framebuffer.h"

Renderer::~Renderer()
{
}

int Renderer::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
		return 1;
	}
}

int Renderer::CreateWindow()
{
	// create window
	// returns pointer to window if successful or nullptr if failed
	SDL_Window* window = SDL_CreateWindow("Game Engine",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// create renderer
	m_renderer = SDL_CreateRenderer(window, -1, 0);

	return 0;
}

void Renderer::CopyFramebuffer(const Framebuffer& framebuffer)
{
	SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
}

void Renderer::operator=(const Framebuffer& framebuffer)
{
	SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
}

