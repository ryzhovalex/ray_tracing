#pragma once

#include <memory>
#include "Walnut/Image.h"
#include "glm/vec2.hpp"

class Renderer
{
public:
	Renderer() = default;
	void resize(uint32_t width, uint32_t height);
	void render();
	std::shared_ptr<Walnut::Image> get_final_img() const
	{
		return final_img;
	}
private:
	std::shared_ptr<Walnut::Image> final_img;
	uint32_t* img_data = nullptr;

	uint32_t per_pixel(glm::vec2 coord);
};
