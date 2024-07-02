#include "renderer.h"
#include "Walnut/Random.h"

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
	for (uint32_t i = 0; i < final_img->GetWidth() * final_img->GetHeight(); i++)
	{
		img_data[i] = Walnut::Random::UInt();
		// set alpha channel to "ff"
		img_data[i] |= 0xff000000;
	}
	final_img->SetData(img_data);
}
