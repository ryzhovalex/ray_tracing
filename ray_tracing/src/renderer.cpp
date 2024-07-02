#include "renderer.h"
#include "Walnut/Random.h"
#include "glm/vec2.hpp"

void Renderer::resize(uint32_t width, uint32_t height)
{
	if (final_img)
	{
		// no resize necessary
		if (width == final_img->GetWidth() && height == final_img->GetHeight())
		{
			return;
		}
		final_img->Resize(width, height);
	}
	else
	{
		final_img = std::make_shared<Walnut::Image>(
			width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] img_data;
	img_data = new uint32_t[width * height];
}

void Renderer::render()
{
	// iterate through y first to optimize cpu caching process (it's easier to
	// through x?)
	for (uint32_t y = 0; y < final_img->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < final_img->GetHeight(); x++)
		{
			glm::vec2 coord = {
				(float)x / (float)final_img->GetWidth(),
				(float)y / (float)final_img->GetHeight()
			};
			img_data[x + y * final_img->GetWidth()] = per_pixel(coord);
		}
	}
	final_img->SetData(img_data);
}

uint32_t Renderer::per_pixel(glm::vec2 coord)
{
	return 0xffff00ff;
}
