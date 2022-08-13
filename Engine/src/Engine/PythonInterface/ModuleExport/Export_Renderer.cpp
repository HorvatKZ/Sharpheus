#include "pch.h"
#include "PydExport.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	void Export_Color(py::module_& handle)
	{
		py::class_<Color>(handle, "Color")
			.def_readwrite("r", &Color::r)
			.def_readwrite("g", &Color::g)
			.def_readwrite("b", &Color::b)
			.def_readwrite("a", &Color::a)

			.def(py::init<>())
			.def(py::init<byte, byte, byte, byte>(), "r"_a, "g"_a, "b"_a, "a"_a = 255)

			.def("get_red", &Color::GetRed)
			.def("get_green", &Color::GetGreen)
			.def("get_blue", &Color::GetBlue)
			.def("get_alpha", &Color::GetAlpha)

			.def("make_opaque", &Color::MakeOpaque, "percent"_a)

			.def_readonly_static("BLACK", &Color::Black)
			.def_readonly_static("RED", &Color::Red)
			.def_readonly_static("GREEN", &Color::Green)
			.def_readonly_static("BLUE", &Color::Blue)
			.def_readonly_static("YELLOW", &Color::Yellow)
			.def_readonly_static("CYAN", &Color::Cyan)
			.def_readonly_static("MAGENTA", &Color::Magenta)
			.def_readonly_static("WHITE", &Color::White)
			.def_readonly_static("TRANSPARENT", &Color::Transparent)
			
			.def("__repr__", [](const Color& c) { return "<Sharpheus.Color (" + std::to_string(c.r) + ", " + std::to_string(c.g) +
				", " + std::to_string(c.b) + ", " + std::to_string(c.a) + ")>"; });
	}


	void Export_Renderer_Module(py::module_& handle)
	{
		py::module_ m = handle.def_submodule("Renderer");
		m.doc() = "Renderer module for Sharpheus";

		m.def("is_inited", &Renderer::IsInited);

		m.def("draw_horizontal_line", &Renderer::DrawHorizontalLine, "begin"_a, "end"_a, "place"_a, "thickness"_a, "color"_a);
		m.def("draw_vertical_line", &Renderer::DrawVerticalLine, "begin"_a, "end"_a, "place"_a, "thickness"_a, "color"_a);
		m.def("draw_line", &Renderer::DrawLine, "begin"_a, "end"_a, "thickness"_a, "color"_a);

		m.def("draw_quad", [](const std::vector<Point>& coords, const std::vector<Point>& texCoords, const Color& tint, uint32 texID) {
			Renderer::DrawQuad(coords.data(), texCoords.data(), tint, texID);
		}, "coords"_a, "tex_coords"_a, "tint"_a, "tex_id"_a);
		m.def("draw_monocrome_quad", [](const std::vector<Point>& coords, const Color& color) {
			Renderer::DrawMonocromeQuad(coords.data(), color);
		}, "coords"_a, "color"_a);

		m.def("draw_circle", [](const std::vector<Point>& coords, const Color& tint) {
			Renderer::DrawCircle(coords.data(), tint);
		}, "coords"_a, "tint"_a);
		m.def("draw_circle_part", [](const std::vector<Point>& coords, const std::vector<Point>& texCoords, const Color& tint) {
			Renderer::DrawCirclePart(coords.data(), texCoords.data(), tint);
		}, "coords"_a, "tex_coords"_a, "tint"_a);

		m.def("get_camera", &Renderer::GetCamera);
		m.def("get_background_color", &Renderer::GetBackgroundColor);

		m.def("is_valid_texture", &Renderer::IsValidTexture, "tex_id"_a);
		m.def("get_invalid_texture", &Renderer::GetInvalidTexture);
		m.def("get_full_tex_coords", [] {
			std::vector<Point> result;
			const Point* coords = Renderer::GetFullTexCoords();
			for (uint8 i = 0; i < 4; ++i) {
				result.push_back(coords[i]);
			}
			return result;
		});

		m.def("get_API_version", &Renderer::GetAPIVersion);
	}


	void Export_Renderer(py::module_& handle)
	{
		Export_Color(handle);
		Export_Renderer_Module(handle);
	}

}