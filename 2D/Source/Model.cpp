#include "Model.h"
#include "Camera.h"
#include "Framebuffer.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool Model::Load(const std::string& filename)
{
	std::ifstream stream(filename);
	if (!stream.is_open()) {
		std::cerr << "error loading model" << std::endl;
		return false;
	}
	vertices_t vertices;
	std::string line;
	while (std::getline(stream, line)) {
		if (line.substr(0) == "v ") {
			std::istringstream sstream{ line.substr(2) };
			glm::vec3 position;
			sstream >> position.x;
			sstream >> position.y;
			sstream >> position.z;
			vertices.push_back(position);
		}
		else if (line.substr(0) == "f ") {
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
					if (index[0] != 0) {
						glm::vec3 position = vertices[index[0] - 1];
						vertices.push_back(position);
					}
				}
			}
		}
	}
	m_vertices = vertices;
	stream.close();
	return true;
}

void Model::SetColor(const color_t& color)
{
	m_color = color;
}

void Model::Draw(Framebuffer& framebuffer, const glm::mat4& model, const Camera& camera)
{
	for (int i = 0; i < m_vertices.size(); i += 3) {
		// convert point from model space to world space space
		vertex_t p1 = model * glm::vec4{ m_vertices[i], 1 };
		vertex_t p2 = model * glm::vec4{ m_vertices[i+1], 1 };
		vertex_t p3 = model * glm::vec4{ m_vertices[i+2], 1 };

		//convert point from world space to view space
		p1 = camera.ModelToView(p1);
		p2 = camera.ModelToView(p2);
		p3 = camera.ModelToView(p3);
		
		//convert point from view space to projection
		glm::ivec2 s1 = camera.ToScreen(p1);
		glm::ivec2 s2 = camera.ToScreen(p2);
		glm::ivec2 s3 = camera.ToScreen(p3);

		framebuffer.DrawTriangle(s1.x, s1.y, s2.x, s2.y, s3.x, s3.y, m_color);
	}
}
