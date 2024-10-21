#include "SDL.h"
#include "Renderer.h"
#include "Framebuffer.h"
#include "PostProcess.h"
#include "Image.h"
#include "Model.h"
#include "Transform.h"
#include "ETime.h"
#include "Input.h"
#include "Camera.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <iostream>

int main(int argc, char* argv[])
{
	Time time;
	Input input;
	input.Initialize();

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow(800, 600);

	Camera camera(800, 600);
	
	camera.SetProjection(60.0f, 800.0f / 600.0f, 0.1f, 200.0f);
	Transform cameraTransform{ {0,0,-20} };

	Framebuffer framebuffer(renderer, 800, 600);
	Image image;
	image.Load("guy.jpg");
	Image imageAlpha;
	imageAlpha.Load("colors.png");
	PostProcess::Alpha(image.m_buffer, 128);

	


	//vertices_t vertices = { {-5,5,0}, {5,5, 0}, {-5,-5,0} };
	//Model model(vertices, {0, 255,0, 255});
	Model model;
	model.Load("teapot.obj");
	model.SetColor({ 0, 255, 0, 255 });

	Transform transform{ {0,0,0}, glm::vec3{0,0,45}, glm::vec3{3} };



	bool quit = false;

	while (!quit)
	{
		time.Tick();
		input.Update();
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

		

		SetBlendMode(BlendMode::Normal);
		//framebuffer.DrawImage(100, 100, imageAlpha);
		//SetBlendMode(BlendMode::Multiply);
		//framebuffer.DrawImage(100, 100, 1.5, 1.5, image);
		//framebuffer.DrawImage(100, 100, image);
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
		glm::vec3 direction{ 0 };
		if (input.GetKeyDown(SDL_SCANCODE_RIGHT)) direction.x = 1;
		if (input.GetKeyDown(SDL_SCANCODE_LEFT)) direction.x = -1;
		if (input.GetKeyDown(SDL_SCANCODE_UP)) direction.y = -1;
		if (input.GetKeyDown(SDL_SCANCODE_DOWN)) direction.y = 1;
		if (input.GetKeyDown(SDL_SCANCODE_W)) direction.z = 4;
		if (input.GetKeyDown(SDL_SCANCODE_S)) direction.z = -4;
		cameraTransform.position += direction * 70.0f * time.GetDeltaTime();
		camera.SetView(cameraTransform.position, cameraTransform.position + glm::vec3 {0, 0, 1});

		transform.rotation.z += 90 * time.GetDeltaTime();
		transform.rotation.y += 85 * time.GetDeltaTime();
		model.Draw(framebuffer, transform.GetMatrix(), camera);
		framebuffer.Update();
		renderer = framebuffer;

		// show screen
		SDL_RenderPresent(renderer.m_renderer);
	}
	
	

	return 0;
}