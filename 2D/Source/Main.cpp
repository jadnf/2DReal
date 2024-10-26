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
#include "Actor.h"
#include "Random.h"



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>


#include <iostream>

int main(int argc, char* argv[])
{
	Time time;

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow(800, 600);

	Input input;
	input.Initialize();
	input.Update();

	SetBlendMode(BlendMode::Alpha);;
	
	Camera camera(800, 600);
	
	camera.SetProjection(30.0f, 800.0f / 600.0f, 5.0f, 200.0f);
	Transform cameraTransform{ {0,0,-20} };

	Framebuffer framebuffer(renderer, 800, 600);
	Image image;
	image.Load("guy.jpg");
	Image imageAlpha;
	imageAlpha.Load("colors.png");
	PostProcess::Alpha(image.m_buffer, 128);

	


	//vertices_t vertices = { {-5,5,0}, {5,5, 0}, {-5,-5,0} };
	//Model model(vertices, {0, 255,0, 255});
	std::shared_ptr<Model> model = std::make_shared<Model>();
	auto model2 = std::make_shared<Model>();
	model2->Load("cube.obj");
	model2->SetColor({ 255,255,0,155 });
	model->Load("sphere.obj");
	model->SetColor({ 0, 255, 0, 255 });

	

	std::vector<std::unique_ptr<Actor>> actors;

	for (int i = 0; i < 6; i++) {
		Transform transform{ {randomf(-10.0f, 10.0f),randomf(-10.0f),0}, glm::vec3{0,randomf(-10.0f),45}, glm::vec3{3}};
		std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, model);
		std::unique_ptr<Actor> actor2 = std::make_unique<Actor>(transform, model2);
		actor->SetColor(color_t{ (uint8_t)random(256), (uint8_t)random(256), (uint8_t)random(256) });
		actor2->SetColor(color_t{ (uint8_t)random(256), (uint8_t)random(256), (uint8_t)random(256) });
		actors.push_back(std::move(actor));
		actors.push_back(std::move(actor2));
	}

	/*Transform transform{ {0,0,0}, glm::vec3{0,0,45}, glm::vec3{3} };
	Actor actor(transform, model);*/



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
		framebuffer.DrawImage(100, 100, imageAlpha);
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
		if (input.GetMouseButtonDown(2)) {
			input.SetRelativeMode(true);
			glm::vec3 direction{ 0 };
			if (input.GetKeyDown(SDL_SCANCODE_D)) direction.x = 1;
			if (input.GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;
			if (input.GetKeyDown(SDL_SCANCODE_E)) direction.y = -1;
			if (input.GetKeyDown(SDL_SCANCODE_Q)) direction.y = 1;
			if (input.GetKeyDown(SDL_SCANCODE_W)) direction.z = 1;
			if (input.GetKeyDown(SDL_SCANCODE_S)) direction.z = -1;
			cameraTransform.rotation.y += input.GetMouseRelative().x * 0.1f;
			cameraTransform.rotation.x += input.GetMouseRelative().y * 0.1f;

			glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4{ direction, 0 };

			cameraTransform.position += offset * 70.0f * time.GetDeltaTime();
		}
		else {
			input.SetRelativeMode(false);
		}
		camera.SetView(cameraTransform.position, cameraTransform.position + cameraTransform.GetForward());

		for (auto& actor : actors) {
			actor->Draw(framebuffer, camera);
		}
		
		

		//transform.rotation.z += 90 * time.GetDeltaTime();
		//transform.rotation.y += 85 * time.GetDeltaTime();
		
		framebuffer.Update();
		renderer = framebuffer;

		// show screen
		SDL_RenderPresent(renderer.m_renderer);
	}
	
	

	return 0;
}