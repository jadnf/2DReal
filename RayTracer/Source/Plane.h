#pragma once
#include "SceneObject.h"
#include <glm/glm.hpp>

class Plane : public SceneObject
{
public:
	Plane() = default;
	Plane(const glm::vec3& center, const glm::vec3& normal, std::shared_ptr<Material> material) :
		SceneObject{ material },		//<call scene object constructor passing material>,
		m_center {center},
		m_normal {normal}
	{
	}

	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) override;

private:
	glm::vec3 m_center;
	glm::vec3 m_normal;
};