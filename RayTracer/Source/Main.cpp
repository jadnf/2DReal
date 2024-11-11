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

void InitScene01(Scene& scene);
void InitScene03(Scene& scene);
void InitScene02(Scene& scene, Camera& camera);

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	Time time;

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("raytrace", 900, 506);

	SetBlendMode(BlendMode::Normal);
	
	Framebuffer framebuffer(renderer, renderer.GetWidth(), renderer.GetHeight());

	Camera camera{ 70.0f, renderer.GetWidth() / (float)(renderer.GetHeight())};
	camera.SetView({ 0,0,-4 }, { 0,0,0 });

	Scene scene;
	scene.SetSky(HSVtoRGB(0.8, 0.7, 0.5), glm::vec3{0.4, 0.6, 0.6});
	//InitScene02(scene, camera);
	//InitScene03(scene);
	InitScene01(scene);

	scene.Update();
	scene.Render(framebuffer, camera, 2500, 15);
	
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
void InitScene01(Scene& scene) {

	std::shared_ptr<Material> red = std::make_shared<Dielectric>(color3_t{ 1,0.5f ,0.5f }, 1.333f);
	std::shared_ptr<Material> green = std::make_shared<Lambertian>(color3_t{ 0,1,0 });
	std::shared_ptr<Material> gray = std::make_shared<Lambertian>(color3_t{ 0.5f });
	std::shared_ptr<Material> gray2 = std::make_shared<Metal>(color3_t{ 0.5f, 0.7f, 0.5f }, 0.1f);
	std::shared_ptr<Material> blue = std::make_shared<Dielectric>(color3_t{ 0.2f, 0.4f, 1 }, 1.9f);
	std::shared_ptr<Material> white = std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1.1f);
	std::shared_ptr<Material> white2 = std::make_shared<Emissive>(color3_t{ 1, 1, 1 }, 1.5f);
	std::shared_ptr<Material> green2 = std::make_shared<Emissive>(color3_t{ 0.2, 1, 0.5 }, 0.2f);
	std::shared_ptr<Material> sunColor = std::make_shared<Emissive>(color3_t{ 0.4, 0.5, 0 }, 50.33f);
	std::shared_ptr<Material> light1 = std::make_shared<Emissive>(color3_t{ 0.1, 0.5, 0.3 }, 3.33f);
	std::vector<std::shared_ptr<Material>> materials;

	materials.push_back(red);
	materials.push_back(green);
	materials.push_back(green2);
	materials.push_back(blue);
	materials.push_back(white);




	auto model = std::make_unique<Model>("models/cube.obj", green, Transform{ glm::vec3{0,0,10}, glm::vec3{45,45,0 }, glm::vec3{3} });
	auto model2 = std::make_unique<Model>("models/cube.obj", white2, Transform{ glm::vec3{4,0,3}, glm::vec3{0,45,0 }, glm::vec3{1.5,4,3} });
	auto model3 = std::make_unique<Model>("models/cube.obj", red, Transform{ glm::vec3{-4,0,3}, glm::vec3{0,-45,0 }, glm::vec3{1.5,4,3} });
	auto plane = std::make_unique<Plane>(Transform{ glm::vec3{0,-2,0}, glm::vec3{0,0,0 } }, gray2);
	auto sun = std::make_unique<Sphere>(glm::vec3{ 10, 10, 0 }, 5.0f, sunColor);
	auto triangle = std::make_unique<Triangle>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 2, 4, 0 }, glm::vec3{ 4, 0, -1 }, Transform{ glm::vec3{-3, 4, 10}, glm::vec3{0}, glm::vec3{0.5} }, light1);
	auto triangle2 = std::make_unique<Triangle>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 6, 4, 0 }, glm::vec3{ 4, 0, 5 }, Transform{ glm::vec3{0, 4, -5}, glm::vec3{0}, glm::vec3{0.5} }, blue);
	scene.AddObject(std::move(plane));
	scene.AddObject(std::move(model));
	scene.AddObject(std::move(model2));
	scene.AddObject(std::move(model3));
	scene.AddObject(std::move(sun));
	scene.AddObject(std::move(triangle));
	scene.AddObject(std::move(triangle2));




	for (int i = 0; i < 80; i++) {
		auto object = std::make_unique<Sphere>(Transform{ random(glm::vec3{ -10 }, glm::vec3{ 10 }) }, randomf(0.2f, 0.7f), std::make_shared<Dielectric>(HSVtoRGB(randomf(50.0f, 255.0f), randomf(0.2f, 1.0f), randomf(0.0f, 1.0f)), randomf(0.2f, 4000.0f)));
		//auto object = std::make_unique<Sphere>(random(glm::vec3{ -6 }, glm::vec3{ 6 }), randomf(0.2f, 1.0f), white);
		scene.AddObject(std::move(object));
	}
}
void InitScene03(Scene& scene)
{
	scene.SetSky(HSVtoRGB(255, 0.4, 0.5), HSVtoRGB(111, 0.1, 0.1));
	std::shared_ptr<Material> white = std::make_shared<Lambertian>(color3_t{ 1,1,1 });
	std::shared_ptr<Material> whiteLight = std::make_shared<Emissive>(color3_t{ 1,1,1 }, 50);
	std::shared_ptr<Material> green = std::make_shared<Lambertian>(color3_t{ 0,1,0 });
	std::shared_ptr<Material> red = std::make_shared<Lambertian>(color3_t{ 1,0,0 });
	std::shared_ptr<Material> clear = std::make_shared<Dielectric>(color3_t{ 1 }, 1.7f);

	auto cube = std::make_unique<Model>("models/cube.obj", white, Transform{ glm::vec3{1.4,-1.5,5}, glm::vec3{0,45,0} });
	auto sphere = std::make_unique<Sphere>(Transform{ glm::vec3{0,1,6} }, 1.0f, clear);
	auto rect = std::make_unique<Model>("models/cube.obj", white, Transform{ glm::vec3{-1.4,-1.5,5}, glm::vec3{0,20,0}, glm::vec3{1,2,1}});
	//auto sphere = std::make_unique<Sphere>(Transform{ glm::vec3{1.4,-1.5,5}, glm::vec3{0,45,0} }, 1.0f, white);
	//auto planeCube2 = std::make_unique<Model>("models/cube.obj", white, Transform{ glm::vec3{0,1,-1}, glm::vec3{0,0,0}, glm::vec3{100,100,0.5} });
	//auto planeBack = std::make_unique<Plane>(Transform{ glm::vec3{0,0.2,10 }, glm::vec3{90,180,0} }, white);
	//auto planeBack = std::make_unique<Model>("models/cube.obj", white, Transform{ glm::vec3{0,1,1}, glm::vec3{0,0,0}, glm::vec3{50,50,0.5} });
	auto planeBack = std::make_unique<Plane>(Transform{ glm::vec3{0,0,10}, glm::vec3{-90,0,0}}, white);
	//auto planeRight = std::make_unique<Model>("models/cube.obj", green, Transform{ glm::vec3{3,0,5}, glm::vec3{0,0,0 }, glm::vec3{0.5,10,20} });
	auto planeRight = std::make_unique<Plane>(Transform{ glm::vec3{3,0,5}, glm::vec3{0,0,90 }}, green);
	//auto planeFloor = std::make_unique<Model>("models/cube.obj", white, Transform{ glm::vec3{0,-2,5}, glm::vec3{0,0,0 }, glm::vec3{2,1,15} });
	auto planeFloor = std::make_unique<Plane>(Transform{ glm::vec3{0,-2,5}, glm::vec3{0,0,0 }, glm::vec3{2,1,15} },white);
	//auto planeLeft = std::make_unique<Model>("models/cube.obj", red, Transform{ glm::vec3{-3,-4,5}, glm::vec3{0,0,0 }, glm::vec3{0.5,15,10} });
	auto planeLeft = std::make_unique<Plane>(Transform{ glm::vec3{-3,-4,5}, glm::vec3{0,0,-90 }}, red);
	//auto planeCeiling = std::make_unique<Model>("models/cube.obj", white, Transform{ glm::vec3{0,3,0}, glm::vec3{0,0,0 }, glm::vec3{100,0.5,100} });
	auto planeCeiling = std::make_unique<Plane>(Transform{ glm::vec3{0,3,0}, glm::vec3{0,0,0 }}, white);
	auto light = std::make_unique<Model>("models/cube.obj", whiteLight, Transform{ glm::vec3{0,3,5}, glm::vec3{0,0,0}, glm::vec3{0.9,0.2,0.9} });

	

	scene.AddObject(std::move(planeRight));
	scene.AddObject(std::move(planeCeiling));
	scene.AddObject(std::move(planeFloor));
	scene.AddObject(std::move(planeLeft));
	scene.AddObject(std::move(planeBack));
	scene.AddObject(std::move(light));
	scene.AddObject(std::move(sphere));
	scene.AddObject(std::move(cube));
	scene.AddObject(std::move(rect));
}
void InitScene02(Scene& scene, Camera& camera)
{
	camera.SetFOV(20.0f);
	camera.SetView({ 13, 2, 3 }, { 0, 0, 0 });

	auto ground_material = std::make_shared<Lambertian>(color3_t(0.5f));
	scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ 0 } }, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = randomf();
			glm::vec3 center(a + 0.9 * randomf(), 0.2, b + 0.9 * randomf());

			if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
					sphere_material = std::make_shared<Lambertian>(albedo);
					scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
					auto fuzz = randomf(0.0f, 0.5f);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
					scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 0, 1, 0 } }, 1.0f, material1));

	auto material2 = std::make_shared<Lambertian>(color3_t(0.4f, 0.2f, 0.1f));
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ -4, 1, 0 } }, 1.0f, material2));

	auto material3 = std::make_shared<Metal>(color3_t(0.7f, 0.6f, 0.5f), 0.0f);
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 4, 1, 0 } }, 1.0f, material3));
}