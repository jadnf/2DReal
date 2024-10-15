#include "SDL.h"
#include "Renderer.h"
#include "Framebuffer.h"
#include "PostProcess.h"
#include "Image.h"
#include "Model.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <iostream>

int main(int argc, char* argv[])
{
	Renderer renderer;
	

	renderer.Initialize();
	renderer.CreateWindow(800, 600);
	Framebuffer framebuffer(renderer, 800, 600);
	Image image;
	image.Load("guy.jpg");
	Image imageAlpha;
	imageAlpha.Load("colors.png");
	PostProcess::Alpha(image.m_buffer, 128);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(40.0f, 40.0f, 0.0f));


	vertices_t vertices = { {-5,5,0}, {5,5, 0}, {-5,-5,0} };
	Model model(vertices, {0, 255,0, 255});



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
		//framebuffer.DrawQuadraticCurve(0, 0, mx, my, 799, 599, { 255,255,255,255 });
		//framebuffer.DrawCubicCurve(0, 0, mx, my, -mx, -my, 799, 599, { 255,255,255,255 });


		/*int ticks = SDL_GetTicks();
		float time = ticks * 0.01f;
		float t = std::abs(std::sin(time));
		int x, y;
		CubicPoint();
			
		framebuffer.drawrect;*/

		//framebuffer.DrawRect(10, 10, 100, 100, { 0,255,0,255 });

		SetBlendMode(BlendMode::Normal);
		framebuffer.DrawImage(100, 100, imageAlpha);
		//SetBlendMode(BlendMode::Alpha);
		//framebuffer.DrawImage(100, 100, 100, 100, image);
		framebuffer.DrawImage(100, 100, image);
#pragma region PostProcess

		//PostProcess::Invert(framebuffer.m_buffer);
		//PostProcess::Posterize(framebuffer.m_buffer, 2);
		//PostProcess::Brightness(framebuffer.m_buffer, 40);
		//PostProcess::um(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::Emboss(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::Crispy(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::Edge(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height, 50);
		//PostProcess::Sharpen(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::GaussianBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::BoxBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::Monochrome(framebuffer.m_buffer);
#pragma endregion
		model.Draw(framebuffer, modelMatrix);
		framebuffer.Update();
		renderer = framebuffer;

		// show screen
		SDL_RenderPresent(renderer.m_renderer);
	}
	
	

	return 0;
}