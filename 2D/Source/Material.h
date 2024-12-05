#pragma once
#include "Color.h"

struct material_t {
	color3_t albedo;
	color3_t specular;
	float shininess = 32.0f;
};