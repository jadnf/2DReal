#pragma once
#include "Color.h"
#include "SceneObject.h"
#include "Transform.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Camera;

using vertex_t = glm::vec3;
using vertices_t = std::vector<vertex_t>;

class Model : public SceneObject{
public:
	Model() = default;
	Model(std::shared_ptr<Material> material) : SceneObject{ material } {}
	Model(const vertices_t& vertices, std::shared_ptr<Material> material) : m_vertices{ vertices }, SceneObject{ material } {}
	Model(const std::string& filename, std::shared_ptr<Material> material) : SceneObject{ material } {
		Load(filename);
	}

	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance);
	bool Load(const std::string& filename);
	
	void SetColor(const color_t& color);
	void Draw(class Framebuffer& framebuffer, const glm::mat4& model, const Camera& camera);
private:
	vertices_t m_vertices;
	color_t m_color = { 0,0,0 };

};