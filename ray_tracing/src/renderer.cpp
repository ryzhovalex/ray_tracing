#include "renderer.h"
#include "Walnut/Random.h"
#include "glm/vec2.hpp"

void Renderer::resize(uint32_t width, uint32_t height)
{
	if (final_img)
	{
		// no resize necessary
		if (
			width == final_img->GetWidth()
			&& height == final_img->GetHeight())
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
		for (uint32_t x = 0; x < final_img->GetWidth(); x++)
		{
			glm::vec2 coord = {
				(float)x / (float)final_img->GetWidth(),
				(float)y / (float)final_img->GetHeight()
			};
			coord = coord * 2.0f - 1.0f;
			// remap coord from range (0,1) to range (-1,1) in order to
			// to achieve anyside direction
			// and yeah, coord here is our ray direction, where we
			// additionally will define z coord which either 1 or -1 depending
			// on camera forward (doesn't matter much for now)
			img_data[x + y * final_img->GetWidth()] = per_pixel(coord);
		}
	}
	final_img->SetData(img_data);
}

uint32_t Renderer::per_pixel(glm::vec2 coord)
{
	auto r = (uint8_t)(coord.x * 255.0f);
	auto g = (uint8_t)(coord.y * 255.0f);

	// move camera back by 2.0f to be able to see the sphere in the middle,
	// and not reside inside it
	glm::vec3 ray_origin(0.0f, 0.0f, 2.0f);
	glm::vec3 ray_dir(coord.x, coord.y, -1.0f);
	glm::normalize(ray_dir);
	float sphere_radius = 0.5f;

	// quadratic equation at^2 + bt + c = 0
	// (d_x^2 + d_y^2) * t^2 + (2 * (o_x * d_x + o_y * d_y)) * t + (o_x^2 + o_y^2 - r^2) = 0
	// where
	// o = ray origin
	// d = ray direction
	// r = radius
	// t = hit distance (scaler)

	// d_x^2 + d_y^2
	//		= sqrt(d_x^2 + d_y^2) * sqrt(d_x^2 + d_y^2)
	//		= ray_dir.magnitude * ray_dir.magnitude * cos(0)
	//		= dot(ray_dir, ray_dir)
	float a = glm::dot(ray_dir, ray_dir);
	// 2 * (o_x * d_x + o_y * d_y)
	//		= 2 * ray_origin.magnitude * ray_dir.magnitude * 1
	//		= 2 * dot(ray_origin, ray_dir)
	// DON'T UNDERSTAND THIS ENTIRELY
	float b = 2.0f * glm::dot(ray_origin, ray_dir);
	// same dot as for "a"
	float c = glm::dot(ray_origin, ray_origin) - pow(sphere_radius, 2);

	// d = b^2 - 4ac
	float d = pow(b, 2) - 4.0f * a * c;
	// if d >= 0, we're sure to have at least one solution, i.e. at least
	// one intersection
	// if we have at least one intersection -> we can draw sphere-coloured
	// pixel
	if (d >= 0.0f)
	{
		return 0xffff00ff;
	}
	return 0xff000000;

	// rgba has 8 bits per color channel, so full rgba is 32 bits per pixel,
	// in this case we use abgr (reversed)
	// alpha=ff + blue = 00 + shifted left green + red
	return 0xff000000 | (g << 8) | r;
}
