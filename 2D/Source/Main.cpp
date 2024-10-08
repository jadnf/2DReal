#include <iostream>
#include "SDL.h"
#include "Renderer.h"
#include "Framebuffer.h"

int main(int argc, char* argv[])
{
	Renderer renderer;

	renderer.Initialize();
	renderer.CreateWindow(800, 600);

	Framebuffer framebuffer(renderer, 400, 300);

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
			int x3 = rand() % framebuffer.m_width;
			int y3 = rand() % framebuffer.m_height;
			//framebuffer.DrawPoint(x, y, { 255,255,255,255 });

			//framebuffer.DrawLine(10, 100, 20, 200, { 255,255,255,255 });
			//framebuffer.CircleBres(10, 100, 20, { 255,255,255,255 });
			//framebuffer.DrawTriangle(10, 100, 20, 200, 132, 60 ,{ 255,255,255,255 });


		

		}
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		//framebuffer.DrawLinearCurve(100, 100, 200, 200, { 255,255,0,255 });
		framebuffer.DrawQuadraticCurve(100, 200, mx, my, 300, 200, { 255,255,255,255 });
		framebuffer.DrawCubicCurve(100, 200, 100, 100, 200, 100, 200, 200, { 255,255,255,255 });

		/*int ticks = SDL_GetTicks();
		float time = ticks * 0.01f;
		float t = std::abs(std::sin(time));
		int x, y;
		CubicPoint();
			
		framebuffer.drawrect;*/

		//framebuffer.DrawRect(10, 10, 100, 100, { 0,255,0,255 });

		framebuffer.Update();
		renderer = framebuffer;

		// show screen
		SDL_RenderPresent(renderer.m_renderer);
	}
	
	

	return 0;
}