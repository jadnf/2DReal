#pragma once
#include <glm/glm.hpp>
#include <random>

inline int random() { return rand(); }
inline int random(unsigned int max) { return rand() % max; }
inline int random(int min, int max) { return min + random(max - min); }

inline float randomf() { return rand() / (float)RAND_MAX; } // 0-1
inline float randomf(float max) { return max * randomf(); }
inline float randomf(float min, float max) { return min + randomf(max - min); }
inline glm::vec3 random(const glm::vec3& v1, const glm::vec3& v2)
{
	return glm::vec3{
		random((int)v1.x, (int)v2.x),
		random((int)v1.y, (int)v2.y),
		random((int)v1.z, (int)v2.z)
	};
}

inline glm::vec2 randomOnUnitCircle() 
{
	float angle = randomf(0, 360);
	return glm::vec2{ glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)) };
}