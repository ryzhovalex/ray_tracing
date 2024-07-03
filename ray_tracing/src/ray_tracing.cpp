#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"
#include "vec3.h"
#include "renderer.h"

class MainLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", last_render_time);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		viewport_width = ImGui::GetContentRegionAvail().x;
		viewport_height = ImGui::GetContentRegionAvail().y;

		auto img = renderer.get_final_img();
		if (img)
		{
			ImGui::Image(
				img->GetDescriptorSet(),
				{ (float)img->GetWidth(), (float)img->GetHeight() },
				// invert UV coords
				ImVec2(0, 1),
				ImVec2(1, 0));
		}

		ImGui::End();
		ImGui::PopStyleVar();

		render();
	}

	void render()
	{
		Walnut::Timer timer;
		renderer.resize(viewport_width, viewport_height);
		renderer.render();
		last_render_time = timer.ElapsedMillis();

		// FORMULAS
		// Sphere:
		//		(x-a)^2 + (y-b)^2 + (z-c)^2 - r^2 = 0
		// Ray:
		//		a + b*t; where a - origin, b - dir, t - scaler
		// Ray-Sphere Intersection (algebra processed):
		//		(b_x^2 + b_y^2 + b_z^2) * t^2 + (2 * a_x * b_x + 2 * a_y * b_y + 2 * a_z * b_z) * t + (a_x^2 + a_y^2 + a_z^2 - r^2) = 0                  
	}

private:
	Renderer renderer;
	uint32_t viewport_width = 0;
	uint32_t viewport_height = 0;
	float last_render_time;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "ray_tracing";

	Walnut::Application* app = new Walnut::Application(spec);

	app->PushLayer<MainLayer>();
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
