#pragma once
#include "Framebuffer.h"

namespace PostProcess {
	void Invert(std::vector<color_t>& buffer);
	void Monochrome(std::vector<color_t>& buffer);
	void Brightness(std::vector<color_t>& buffer, int brightness);
	void Posterize(std::vector<color_t>& buffer, uint8_t levels);
	void Alpha(std::vector<color_t>& buffer, uint8_t alpha);
	void BoxBlur(std::vector<color_t>& buffer, int w, int h);
	void GaussianBlur(std::vector<color_t>& buffer, int w, int h);
	void Sharpen(std::vector<color_t>& buffer, int w, int h);
	void Edge(std::vector<color_t>& buffer, int w, int h, int threshold);
	void um(std::vector<color_t>& buffer, int w, int h);
	void Crispy(std::vector<color_t>& buffer, int width, int height);
	void Emboss(std::vector<color_t>& buffer, int width, int height);
}