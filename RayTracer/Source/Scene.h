#pragma once
#include "Sphere.h"
#include "SceneObject.h"
#include "Triangle.h"
#include "Model.h"
#include <vector>
#include <memory>

class Scene {
public:
	Scene() = default;

	void Update();

	void Render(class Framebuffer& framebuffer, const class Camera& camera, int numSamples = 10, int depth = 5);
	void AddObject(std::unique_ptr<class SceneObject> object) { m_objects.push_back(std::move(object)); }

	void SetSky(const color3_t& skyTop, const color3_t& skyBottom) {
		m_skyTop = skyTop;
		m_skyBottom = skyBottom;
	}

	friend class Tracer;
private:
	std::vector<std::unique_ptr<class SceneObject>> m_objects;
	color3_t m_skyTop{1};
	color3_t m_skyBottom{ 0.5f };
};