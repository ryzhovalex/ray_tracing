#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", last_render_time);
		if (ImGui::Button("Render"))
		{
			render();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		viewport_width = ImGui::GetContentRegionAvail().x;
		viewport_height = ImGui::GetContentRegionAvail().y;
		if (img)
		{
			ImGui::Image(
				img->GetDescriptorSet(),
				{ (float)img->GetWidth(), (float)img->GetHeight() });
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void render()
	{
		Timer timer;

		// we also recreate img if it needs resizing
		if (
			!img
			|| viewport_width != img->GetWidth()
			|| viewport_height != img->GetHeight())
		{
			img = std::make_shared<Image>(
				viewport_width,
				viewport_height,
				ImageFormat::RGBA);

			// reallocate enough size to store all pixels
			delete[] image_data;
			image_data = new uint32_t[viewport_width * viewport_height];
		}

		for (uint32_t i = 0; i < viewport_width * viewport_height; i++)
		{
			// ABGR (A = ff, B = ff, G = 00, R = ff) format
			//image_data[i] = 0xffff00ff;

			image_data[i] = Random::UInt();
			// set alpha channel to "ff"
			image_data[i] |= 0xff000000;
		}

		img->SetData(image_data);
		last_render_time = timer.ElapsedMillis();
	}

private:
	std::shared_ptr<Image> img;
	uint32_t* image_data = nullptr;
	uint32_t viewport_width = 0, viewport_height = 0;
	float last_render_time = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "ray_tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}
