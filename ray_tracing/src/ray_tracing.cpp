#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"
#include "vec3.h"

using namespace Walnut;

class MainLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::InputFloat("sphere radius", &sphere_radius);
		ImGui::Text(std::format("(1) {}", text1).c_str());
		ImGui::Text(std::format("(2) {}", text2).c_str());
		ImGui::Text(std::format("(3) {}", text3).c_str());
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
		text3 = sphere_radius;

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

		Vec3 cam_pos(0, 0, -1000);
		Vec3 sphere_pos(0, 0, 0);
		// FORMULAS
		// Sphere:
		//		(x-a)^2 + (y-b)^2 + (z-c)^2 - r^2 = 0
		// Ray:
		//		a + b*t; where a - origin, b - dir, t - scaler
		// Ray-Sphere Intersection:
		//		(b_x^2 + b_y^2 + b_z^2) * t^2 + (2 * a_x * b_x + 2 * a_y * b_y + 2 * a_z * b_z) * t + (a_x^2 + a_y^2 + a_z^2 - r^2) = 0                  
		Vec3 screen_center(
			get_screen_center_cord(viewport_width),
			get_screen_center_cord(viewport_height),
			0.0f);
		text1 = std::format(
			"viewport: {}:{}", viewport_width, viewport_height);
		text2 = std::format(
			"screen center: {}",
			screen_center.to_str());
		float a_x = cam_pos.get_x();
		float a_y = cam_pos.get_y();
		float a_z = cam_pos.get_z();
		int quadratic_c = 
			pow(a_x, 2) + pow(a_y, 2) + pow(a_z, 2) - pow(sphere_radius, 2);
		for (uint32_t i = 0; i < viewport_width * viewport_height; i++)
		{
			Vec3 screen_pos(
				i % viewport_width,
				(int)i / viewport_height,
				0.0f);
			// offset from screen center to screen point is the same as offset
			// from world center to projected world point, in our context
			//
			// but also we need to do Y inversion, since cord system is
			// rotated differently for world space
			Vec3 world_pos = screen_pos - screen_center;
			world_pos.data[1] *= -1;
			Vec3 ray_dir = (world_pos - cam_pos).normalized();

			// if intersection quadractic equation gives discriminant >= 0,
			// we know that intersection has been happened at least in one
			// point, so we draw sphere pixel there
			float b_x = ray_dir.get_x();
			float b_y = ray_dir.get_y();
			float b_z = ray_dir.get_z();
			// d = b**2 - 4ac
			int d =
				pow(2 * a_x * b_x + 2 * a_y * b_y + 2 * a_z * b_z, 2)
				- 4 * (pow(b_x, 2) + pow(b_y, 2) + pow(b_z, 2)) * quadratic_c;
			if (d >= 0)
			{
				image_data[i] = SPHERE_COLOR;
				continue;
			}
			image_data[i] = BG_COLOR;

			// set alpha channel to "ff"
			//image_data[i] |= 0xff000000;
		}

		img->SetData(image_data);
		last_render_time = timer.ElapsedMillis();
	}

	int get_screen_center_cord(int len)
	{
		if (len % 2 == 0)
		{
			return ((int)len / 2) - 1;
		}
		return ((int)len / 2) + 1;
	}

private:
	std::shared_ptr<Image> img;
	uint32_t* image_data = nullptr;
	uint32_t viewport_width = 0, viewport_height = 0;
	float last_render_time = 0.0f;

	std::string text1;
	std::string text2;
	std::string text3;

	// ABGR (A = ff, B = ff, G = 00, R = ff) format
	const uint32_t SPHERE_COLOR = 0xff00ffff;
	const uint32_t BG_COLOR = 0xffebce87;

	float sphere_radius = 2;
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
