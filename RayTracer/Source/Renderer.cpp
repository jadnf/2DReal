#include "Renderer.h"
#include "Framebuffer.h"
#include <string>

Renderer::~Renderer()
{
}

int Renderer::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

int Renderer::CreateWindow(std::string name, int w, int h)
{
	m_width = w;
	m_height = h;
	// create window
	// returns pointer to window if successful or nullptr if failed
	m_window = SDL_CreateWindow(name.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		w, h,
		SDL_WINDOW_SHOWN);
	if (m_window == nullptr)
	{
		std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	// create renderer
	m_renderer = SDL_CreateRenderer(m_window, -1, 0);

	return true;
}

void Renderer::CopyFramebuffer(const Framebuffer& framebuffer)
{
	SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
}

void Renderer::operator=(const Framebuffer& framebuffer)
{
	SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
}

