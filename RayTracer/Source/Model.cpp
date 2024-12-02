#include "Model.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Triangle.h"
#include "Sphere.h"

#include <fstream>
#include <iostream>
#include <sstream>

void Model::Update()
{
	for (size_t i = 0; i < m_local_vertices.size(); i++) {
		m_vb[i] = m_transform * glm::vec4{ m_local_vertices[i], 1 };
	}
	// get center point of transformed vertices
	m_center = glm::vec3{ 0 };
	for (auto& vertex : m_vb)
	{
		m_center += vertex;
	}
	m_center /= (float)m_vb.size();

	m_radius = 0;
	for (auto& vertex : m_vb)
	{
		float radius = glm::length(vertex - m_center);//<use glm::length of the vector(vertex - m_center)>;
		m_radius = glm::max(radius, m_radius);//<use glm::max of the radius and m_radius>
	}
}

bool Model::Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance)
{
	float t;
	if (!Sphere::Raycast(ray, m_center, m_radius, minDistance, maxDistance, t)) return false;
	// check cast ray with mesh triangles 
	for (size_t i = 0; i < m_vb.size(); i += 3)
	{
		float t;
		if (Triangle::Raycast(ray, m_vb[i], m_vb[i + 1], m_vb[i + 2], minDistance, maxDistance, t))
		{
			raycastHit.distance = t;
			raycastHit.point = ray.At(t);
			glm::vec3 edge1 = m_vb[i+1] - m_vb[i];
			glm::vec3 edge2 = m_vb[i + 2] - m_vb[i];

			raycastHit.normal = glm::normalize(glm::cross(edge1, edge2)); //<calculate triangle normal, vector perpendicular to edge1 and edge 2, normalize vector>
			raycastHit.material = GetMaterial();
			return true;
		}
	}

	return false;
}

bool Model::Load(const std::string& filename)
{
	std::ifstream stream(filename);
	if (!stream.is_open()) {
		std::cerr << "error loading model:" << filename << std::endl;
		return false;
	}
	vertexbuffer_t vertices;
	std::string line;
	while (std::getline(stream, line)) {
		if (line.substr(0, 2) == "v ") {
			std::istringstream sstream{ line.substr(2) };
			glm::vec3 position;
			sstream >> position.x;
			sstream >> position.y;
			sstream >> position.z;
			vertices.push_back(position);
		}
		else if (line.substr(0,2) == "f ") {
			std::istringstream sstream{ line.substr(2) };
			std::string str;
			while (std::getline(sstream, str, ' ')) {
				std::istringstream sstream(str);
				std::string indexString;

				size_t i = 0;
				unsigned int index[3] = { 0, 0, 0 };
				while (std::getline(sstream, indexString, '/')) {
					if (!indexString.empty()) {
						std::istringstream indexStream{ indexString };
						indexStream >> index[i];
					}
					i++;
				}
				if (index[0]) {
					glm::vec3 position = vertices[index[0] - 1];
					m_local_vertices.push_back(position);
					
				}
				
			}
		}
	}
	m_vb.resize(m_local_vertices.size());
	
	stream.close();
	return true;
}

void Model::SetColor(const color_t& color)
{
	m_color = color;
}

void Model::Draw(Framebuffer& framebuffer, const glm::mat4& model, const Camera& camera)
{
	
}
