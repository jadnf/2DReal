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
#include "VertexShader.h"
#include "Shader.h"



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
	
	Camera camera(800, 600);
	camera.SetView(glm::vec3{ 0, 0, -20 }, glm::vec3{ 0 });
	camera.SetProjection(30.0f, 800.0f / 600.0f, 0.1f, 200.0f);
	Transform cameraTransform{ {0,0,-20} };

	Framebuffer framebuffer(renderer, 800, 600);

	// materials
	std::shared_ptr<material_t> material = std::make_shared<material_t>();
	material->albedo = color3_t{ 0, 0, 1 };
	material->specular = color3_t{ 1 };
	material->shininess = 32.0f;

	//shader
	Shader::uniforms.view = camera.GetView();
	Shader::uniforms.projection = camera.GetProjection();
	Shader::uniforms.ambient = color3_t{ 0.1f };

	Shader::uniforms.light.position = glm::vec3{ -6, 10, 1 };
	Shader::uniforms.light.direction = glm::vec3{ 0, -1, 0 }; // light pointing down
	Shader::uniforms.light.color = color3_t{ 1 }; // white light

	Shader::framebuffer = &framebuffer;


	//vertices_t vertices = { {-5,5,0}, {5,5, 0}, {-5,-5,0} };
	//Model model(vertices, {0, 255,0, 255});
	auto model = std::make_shared<Model>();
	model->Load("models/cube.obj");
	model->SetColor({ 0,0,1,1 });
	

	

	std::vector<std::unique_ptr<Actor>> actors;

	Transform transform{ glm::vec3{0, 0, 0},glm::vec3{0},glm::vec3{5} };
	std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, model, material);
	actors.push_back(std::move(actor));
	

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
		Shader::uniforms.view = camera.GetView();

		for (auto& actor : actors) {
			actor->GetTransform().rotation.y += time.GetDeltaTime() * 90;
			actor->Draw();
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