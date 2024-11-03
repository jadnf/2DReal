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
#include "Triangle.h"

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
	camera.SetView({ 5,3,-5 }, { 0,0,0 });

	Scene scene;

	std::shared_ptr<Material> red   = std::make_shared<Dielectric>(color3_t{ 1,0.5f ,0.5f }, 1.333f);
	std::shared_ptr<Material> green = std::make_shared<Metal>(color3_t{ 0,1,0 }, 0.8f);
	std::shared_ptr<Material> gray  = std::make_shared<Metal>(color3_t{ 0.5f }, 0.0f);
	std::shared_ptr<Material> gray2  = std::make_shared<Metal>(color3_t{ 0.5f, 0.7f, 0.5f }, 0.01f);
	std::shared_ptr<Material> blue  = std::make_shared<Dielectric>(color3_t{ 0.2f, 0.4f, 1 }, 1.9f);
	std::shared_ptr<Material> white  = std::make_shared<Dielectric>(color3_t{ 1, 1, 1 },2.33f);
	std::shared_ptr<Material> green2  = std::make_shared<Emissive>(color3_t{ 0.2, 1, 0.5 },1.33f);
	std::shared_ptr<Material> sunColor  = std::make_shared<Emissive>(color3_t{ 0.9, 0.5, 0 },50000000.33f);
	std::vector<std::shared_ptr<Material>> materials;

	materials.push_back(red);
	materials.push_back(green);
	materials.push_back(green2);
	materials.push_back(blue);
	materials.push_back(white);

	

	
	auto model = std::make_unique<Model>("cube.obj", green);
	//model->
	auto plane = std::make_unique<Plane>(glm::vec3{ 0, -3, 0 }, glm::vec3{ 0, 1, 0 }, gray);
	auto sun = std::make_unique<Sphere>(glm::vec3{ 10, 10, 0 }, 5.0f, sunColor);
	auto triangle = std::make_unique<Triangle>(glm::vec3{ -4, 0, 0 }, glm::vec3{ 0, 4, 0 }, glm::vec3{ 4, 0, 0 }, green2);
	auto triangle2 = std::make_unique<Triangle>(glm::vec3{ -4, 0, 10 }, glm::vec3{ 0, 4, -10 }, glm::vec3{ 4, 0, 10 }, green);
	scene.AddObject(std::move(plane));
	scene.AddObject(std::move(model));
	scene.AddObject(std::move(sun));
	/*scene.AddObject(std::move(triangle));
	scene.AddObject(std::move(triangle2));*/

	auto mainsphere = std::make_unique<Sphere>(glm::vec3{ 2, 0, 0 }, 0.4f, blue);
	scene.AddObject(std::move(mainsphere));
	auto mainsphere2 = std::make_unique<Sphere>(glm::vec3{ -2, 0, 0 }, 0.4f, green2);
	scene.AddObject(std::move(mainsphere2));
	auto mainsphere3 = std::make_unique<Sphere>(glm::vec3{ 0, 0, -2 }, 0.4f, white);
	scene.AddObject(std::move(mainsphere3));
	auto mainsphere4 = std::make_unique<Sphere>(glm::vec3{ 0, 0, 2 }, 0.4f, gray2);
	scene.AddObject(std::move(mainsphere4));
	//auto mainsphere3 = std::make_unique<Sphere>(glm::vec3{ -10, 0, 0 }, 3.0f, red);
	//scene.AddObject(std::move(mainsphere3));

	
	//for (int i = 0; i < 0; i++) {
	//	auto object = std::make_unique<Sphere>(random(glm::vec3{ -10 }, glm::vec3{ 10 }), randomf(0.2f, 3.0f), materials[random(0, (int)materials.size())]);
	//	//auto object = std::make_unique<Sphere>(random(glm::vec3{ -10 }, glm::vec3{ 10 }), randomf(0.2f, 3.0f), white);
	//	scene.AddObject(std::move(object));
	//}
	scene.Render(framebuffer, camera, 500, 30);
	
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
		framebuffer.Update();

		//scene.Render(framebuffer, camera, 2, 4);
		framebuffer.Update();

		renderer = framebuffer;
		SDL_RenderPresent(renderer.m_renderer);
	}
	
	

	return 0;
}