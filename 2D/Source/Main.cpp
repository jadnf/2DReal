#include "SDL.h"
#include "Renderer.h"
#include "Framebuffer.h"
#include "PostProcess.h"
#include "Image.h"

#include <iostream>

int main(int argc, char* argv[])
{
	Renderer renderer;
	

	renderer.Initialize();
	renderer.CreateWindow(800, 600);
	Image image;
	image.Load("guy.jpg");

	Framebuffer framebuffer(renderer, 800, 600);

	bool quit = false;

	while (!quit)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}
		framebuffer.Clear(color_t{ 1,0,0,255 });

		
		int mx, my;
		SDL_GetMouseState(&mx, &my);

		//framebuffer.DrawTriangle(50, 0, 400, 300, 50, 300,{ 255,255,255,255 });
		//framebuffer.DrawLineSlope(0, 0, 799, 599, { 255,255,255,255 });
		//framebuffer.DrawCircle(50, 100, 20, { 255,255,255,255 });
		//framebuffer.DrawLinearCurve(0, 0, mx, my, { 255,255,0,255 });
		framebuffer.DrawImage(0, 0, image);
		//framebuffer.DrawQuadraticCurve(0, 0, mx, my, 799, 599, { 255,255,255,255 });
		//framebuffer.DrawCubicCurve(0, 0, mx, my, -mx, -my, 799, 599, { 255,255,255,255 });


		/*int ticks = SDL_GetTicks();
		float time = ticks * 0.01f;
		float t = std::abs(std::sin(time));
		int x, y;
		CubicPoint();
			
		framebuffer.drawrect;*/

		//framebuffer.DrawRect(10, 10, 100, 100, { 0,255,0,255 });
		//PostProcess::Invert(framebuffer.m_buffer);
		//PostProcess::Brightness(framebuffer.m_buffer);
		framebuffer.Update();
		renderer = framebuffer;

		// show screen
		SDL_RenderPresent(renderer.m_renderer);
	}
	
	

	return 0;
}