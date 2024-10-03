#include <iostream>
#include "SDL.h"
#include "Renderer.h"
#include "Framebuffer.h"

int main(int argc, char* argv[])
{
	Renderer renderer;

	renderer.Initialize();
	renderer.CreateWindow();

	Framebuffer framebuffer(renderer, 200, 150);

	while (true)
	{
		// clear screen
		/*SDL_SetRenderDrawColor(renderer.m_renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer.m_renderer);*/

		framebuffer.Clear(color_t{ 1,0,0,255 });

		for (int i = 0; i < 100; i++) {
			int x = rand() % framebuffer.m_width;
			int y = rand() % framebuffer.m_height;
			int x2 = rand() % framebuffer.m_width;
			int y2 = rand() % framebuffer.m_height;
			framebuffer.DrawPoint(x, y, { 255,255,255,255 });

			//framebuffer.DrawLine(x, y, x2, y2, { 255,255,255,255 });
		}

		framebuffer.DrawRect(10, 10, 100, 100, { 0,255,0,255 });

		framebuffer.Update();
		renderer = framebuffer;

		// show screen
		SDL_RenderPresent(renderer.m_renderer);
	}
	
	

	return 0;
}