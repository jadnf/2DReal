#pragma once
#include "SDL.h"
#include <iostream>


class Renderer {
public:
	Renderer() = default;
	~Renderer();

	int Initialize();

	int CreateWindow(int w, int h);

	void CopyFramebuffer(const class Framebuffer& framebuffer);
	void operator = (const class Framebuffer& framebuffer);



	SDL_Renderer* m_renderer{ nullptr };
private:
};