#include "Model.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Triangle.h"

#include <fstream>
#include <iostream>
#include <sstream>

bool Model::Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance)
{
	// check cast ray with mesh triangles 
	for (size_t i = 0; i < m_vertices.size(); i += 3)
	{
		Triangle triangle(m_vertices[i], m_vertices[i +1], m_vertices[i+2], m_material); //<create triangle with the three vertex points>
		if (triangle.Hit(ray, raycastHit, minDistance, maxDistance))
		{
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
	vertices_t vertices;
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
					m_vertices.push_back(position);
					
				}
				
			}
		}
	}
	
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
