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
#include "Tracer.h"
#include "Material.h"
#include "Plane.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	Time time;

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("raytrace", 800, 600);

	SetBlendMode(BlendMode::Normal);
	
	Framebuffer framebuffer(renderer, renderer.GetWidth(), renderer.GetHeight());

	Camera camera{ 70.0f, renderer.GetWidth() / (float)(renderer.GetHeight())};
	camera.SetView({ 0,0,-20 }, { 0,0,0 });

	Scene scene;

	std::shared_ptr<Material> red   = std::make_shared<Lambertian>(color3_t{ 1,0,0 });
	std::shared_ptr<Material> green = std::make_shared<Metal>(color3_t{ 0,1,0 }, 0.3f);
	std::shared_ptr<Material> gray  = std::make_shared<Lambertian>(color3_t{ 0.5f });
	std::shared_ptr<Material> blue  = std::make_shared<Metal>(color3_t{ 0, 0, 1 },0.0f);
	std::vector<std::shared_ptr<Material>> materials;
	materials.push_back(red);
	materials.push_back(green);
	materials.push_back(blue);
	//materials.push_back(gray);

	auto plane = std::make_unique<Plane>(glm::vec3{ 0, -3, 0 }, glm::vec3{ 0, 1, 0 }, gray);
	scene.AddObject(std::move(plane));
	
	for (int i = 0; i < 10; i++) {
		auto object = std::make_unique<Sphere>(random(glm::vec3{ -10 }, glm::vec3{ 10 }), randomf(0.2f, 3.0f), materials[random(0, (int)materials.size())]);
		scene.AddObject(std::move(object));
	}
	
	bool quit = false;
	while (!quit)
	{
		time.Tick();
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
		//framebuffer.Clear(ColorConvert(color4_t{ 0,0.25f,0,255 }));

		scene.Render(framebuffer, camera);
		framebuffer.Update();

		renderer = framebuffer;
		SDL_RenderPresent(renderer.m_renderer);
	}
	
	

	return 0;
}