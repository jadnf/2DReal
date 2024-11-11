#pragma once
#include "SceneObject.h"
#include "Ray.h"
#include <glm/glm.hpp>

class Sphere : public SceneObject {
public:
	Sphere() = default;
	Sphere(const Transform& transform, float radius, std::shared_ptr<Material> material) :
		SceneObject{ transform, material },
		m_radius{ radius }
	{}
	// Inherited via SceneObject
	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) override;
	static bool Raycast(const ray_t& ray, const glm::vec3& center, float radius, float mindist, float maxdist, float& t);


private:
	//glm::vec3 m_center;
	float m_radius{ 0 };
};