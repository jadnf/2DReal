#include "PostProcess.h"
#include "MathUtils.h"
#include <algorithm>
#include <cmath>

void PostProcess::Invert(std::vector<color_t>& buffer)
{
	/*for (auto& color : buffer) {
		color.r = 255 - color.r;
		color.g = 255 - color.g;
		color.b = 255 - color.b;
	}*/
	
	std::for_each(buffer.begin(), buffer.end(), [](auto& c) {
		c.r = 255 - c.r;
		c.g = 255 - c.g;
		c.b = 255 - c.b;
		});
}

void PostProcess::Monochrome(std::vector<color_t>& buffer)
{
	std::for_each(buffer.begin(), buffer.end(), [](auto& c) {
		uint8_t average = static_cast<uint8_t>((c.r + c.g + c.b) / 3);
		c.r = average;
		c.g = average;
		c.b = average;
		});
	
}

void PostProcess::Brightness(std::vector<color_t>& buffer, int brightness)
{
	std::for_each(buffer.begin(), buffer.end(), [brightness](auto& c) {
		c.r = static_cast<uint8_t>(Clamp(c.r + brightness, 0, 255));
		c.g = static_cast<uint8_t>(Clamp(c.g + brightness, 0, 255));
		c.b = static_cast<uint8_t>(Clamp(c.b + brightness, 0, 255));
		});
}

void PostProcess::Posterize(std::vector<color_t>& buffer, uint8_t levels)
{
	uint8_t level = 255 / levels;
	std::for_each(buffer.begin(), buffer.end(), [level](auto& c) {
		c.r = (c.r / level) * level;
		c.g = (c.g / level) * level;
		c.b = (c.b / level) * level;
		});
}

void PostProcess::Alpha(std::vector<color_t>& buffer, uint8_t alpha)
{
	std::for_each(buffer.begin(), buffer.end(), [alpha](auto& c) {
		c.a = alpha;
		});
}

void PostProcess::BoxBlur(std::vector<color_t>& buffer, int w, int h)
{
	std::vector<color_t> source = buffer;

	int kern[3][3] = {
		{1,1,1},
		{1,1,1},
		{1,1,1}
	};
	for (int i = 0; i < buffer.size(); i++) {
		int x = i % w;
		int y = i / w;

		//skip if out of range
		if (x < 1 || x + 1 >= w || y < 1 || y + 1 >= h) continue;

		int r = 0;
		int g = 0;
		int b = 0;

		for (int iy = 0; iy < 3; iy++) {
			for (int ix = 0; ix < 3; ix++) {
				color_t pixel = source[(x + ix - 1) + (y + iy -1) * w];
				int weight = kern[iy][ix];
				r += pixel.r * weight;
				g += pixel.g * weight;
				b += pixel.b * weight;
			}
		}
		color_t& color = buffer[i];
		color.r = static_cast<uint8_t>(r / 9);
		color.g = static_cast<uint8_t>(g / 9);
		color.b = static_cast<uint8_t>(b / 9);
	}
}

void PostProcess::GaussianBlur(std::vector<color_t>& buffer, int w, int h)
{
	std::vector<color_t> source = buffer;

	int kern[3][3] = {
		{1,2,1},
		{2,4,2},
		{1,2,1}
	};
	for (int i = 0; i < buffer.size(); i++) {
		int x = i % w;
		int y = i / w;

		//skip if out of range
		if (x < 1 || x + 1 >= w || y < 1 || y + 1 >= h) continue;

		int r = 0;
		int g = 0;
		int b = 0;

		for (int iy = 0; iy < 3; iy++) {
			for (int ix = 0; ix < 3; ix++) {
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * w];
				int weight = kern[iy][ix];
				r += pixel.r * weight;
				g += pixel.g * weight;
				b += pixel.b * weight;
			}
		}
		color_t& color = buffer[i];
		color.r = static_cast<uint8_t>(r / 16);
		color.g = static_cast<uint8_t>(g / 16);
		color.b = static_cast<uint8_t>(b / 16);
	}
}

void PostProcess::Sharpen(std::vector<color_t>& buffer, int w, int h)
{
	std::vector<color_t> source = buffer;

	int kern[3][3] = {
		{0,-1,0},
		{-1,5,-1},
		{0,-1,0}
	};
	for (int i = 0; i < buffer.size(); i++) {
		int x = i % w;
		int y = i / w;

		//skip if out of range
		if (x < 1 || x + 1 >= w || y < 1 || y + 1 >= h) continue;

		int r = 0;
		int g = 0;
		int b = 0;

		for (int iy = 0; iy < 3; iy++) {
			for (int ix = 0; ix < 3; ix++) {
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * w];
				int weight = kern[iy][ix];
				r += pixel.r * weight;
				g += pixel.g * weight;
				b += pixel.b * weight;
			}
		}
		color_t& color = buffer[i];
		color.r = static_cast<uint8_t>(Clamp(r, 0, 255));
		color.g = static_cast<uint8_t>(Clamp(g, 0, 255));
		color.b = static_cast<uint8_t>(Clamp(b, 0, 255));
	}
}

void PostProcess::Edge(std::vector<color_t>& buffer, int w, int h, int threshold)
{
	std::vector<color_t> source = buffer;
	Monochrome(buffer);

	int hkern[3][3] = {
		{1,0,-1},
		{2,0,-2},
		{1,0,-1}
	};
	int vkern[3][3] = {
		{-1,-2,-1},
		{0,0,0},
		{1,2,1}
	};
	for (int i = 0; i < buffer.size(); i++) {
		int x = i % w;
		int y = i / w;

		//skip if out of range
		if (x < 1 || x + 1 >= w || y < 1 || y + 1 >= h) continue;

		int h = 0;
		int v = 0;

		for (int iy = 0; iy < 3; iy++) {
			for (int ix = 0; ix < 3; ix++) {
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * w];
				h += pixel.r * hkern[iy][ix];
				v += pixel.r * vkern[iy][ix];
			}
		}
		int m = (int)std::sqrt(h * h + v * v);
		m = (m >= threshold) ? m : 0;

		uint8_t c = std::clamp(m, 0, 255);

		color_t& color = buffer[i];
		color.r = c;
		color.g = c;
		color.b = c;
		//color.r = static_cast<uint8_t>(Clamp(r, 0, 255));
		//color.g = static_cast<uint8_t>(Clamp(g, 0, 255));
		//color.b = static_cast<uint8_t>(Clamp(b, 0, 255));
	}
}

void PostProcess::um(std::vector<color_t>& buffer, int w, int h)
{
	std::vector<color_t> source = buffer;

	int kern[3][3] = {
		{1,2,1},
		{2,4,2},
		{1,2,1}
	};
	for (int i = 0; i < buffer.size(); i++) {
		int x = i % w;
		int y = i / w;

		//skip if out of range
		if (x < 1 || x + 1 >= w || y < 1 || y + 1 >= h) continue;

		int r = 0;
		int g = 0;
		int b = 0;

		for (int iy = 0; iy < 3; iy++) {
			for (int ix = 0; ix < 3; ix++) {
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * w];
				int weight = kern[iy][ix];
				r += pixel.r * weight;
				g += pixel.g * weight;
				b += pixel.b * weight;
			}
		}
		color_t& color = buffer[i];
		color.r = static_cast<uint8_t>(r / 9);
		color.g = static_cast<uint8_t>(g / 9);
		color.b = static_cast<uint8_t>(b / 9);
	}
}

void PostProcess::Crispy(std::vector<color_t>& buffer, int w, int h)
{
	std::vector<color_t> source = buffer;

	int kern[3][3] = {
		{0,1,0},
		{0,0,0},
		{0,-1,0}
	};
	for (int i = 0; i < buffer.size(); i++) {
		int x = i % w;
		int y = i / w;

		//skip if out of range
		if (x < 1 || x + 1 >= w || y < 1 || y + 1 >= h) continue;

		int r = 0;
		int g = 0;
		int b = 0;

		for (int iy = 0; iy < 3; iy++) {
			for (int ix = 0; ix < 3; ix++) {
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * w];
				int weight = kern[iy][ix];
				r += pixel.r * weight;
				g += pixel.g * weight;
				b += pixel.b * weight;
			}
		}
		color_t& color = buffer[i];
		color.r = static_cast<uint8_t>(r);
		color.g = static_cast<uint8_t>(g);
		color.b = static_cast<uint8_t>(b);
	}
}
void PostProcess::Emboss(std::vector<color_t>& buffer, int w, int h)
{
	std::vector<color_t> source = buffer;

	int kern[3][3] = {
		/*{-2,-1,0},
		{-1,1,1},
		{0,1,2}*/
		{0,1,0},
		{0,0,0},
		{0,-1,0}
	};
	for (int i = 0; i < buffer.size(); i++) {
		int x = i % w;
		int y = i / w;

		//skip if out of range
		if (x < 1 || x + 1 >= w || y < 1 || y + 1 >= h) continue;

		int r = 0;
		int g = 0;
		int b = 0;

		for (int iy = 0; iy < 3; iy++) {
			for (int ix = 0; ix < 3; ix++) {
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * w];
				int weight = kern[iy][ix];
				r += pixel.r * weight;
				g += pixel.g * weight;
				b += pixel.b * weight;
			}
		}
		color_t& color = buffer[i];
		color.r = std::clamp(r + 128, 0, 255);
		color.g = std::clamp(g + 128, 0, 255);
		color.b = std::clamp(b + 128, 0, 255);
	}
}
