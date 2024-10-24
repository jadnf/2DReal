#pragma once

class Tracer {
public:
	Tracer() = default;

	void Render(class Framebuffer& framebuffer, const class Camera& camera);
private:
};