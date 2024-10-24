#include "Tracer.h"
#include "Framebuffer.h"
#include "Camera.h"

void Tracer::Render(class Framebuffer& framebuffer, const class Camera& camera)
{
	for (int y = 0; y < framebuffer.m_height; y++) {
		for (int x = 0; x < framebuffer.m_width; x++) {
			glm::vec2 point{ x,y };

			ray_t ray = camera.GetRay(point);

			color4_t color = { 1,0,0,1 };
			framebuffer.DrawPoint(x, y, ColorConvert(color));
		}
	}
}
