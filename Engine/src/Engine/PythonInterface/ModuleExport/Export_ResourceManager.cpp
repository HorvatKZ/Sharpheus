#include "pch.h"
#include "PydExport.hpp"
#include "Engine/PythonInterface/NonCopyPyCast.hpp"
#include "Engine/ResourceManager/AudioPlayer.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	void Export_Resource(py::module_& handle)
	{
		py::class_<Resource>(handle, "Resource")
			.def_property_readonly("is_valid", &Resource::IsValid)
			.def_property_readonly("has_path", &Resource::HasPath)
			.def_property_readonly("path", &Resource::GetPath)
			.def_property_readonly("full_path", &Resource::GetFullPath);
	}


	void Export_Image(py::module_& handle)
	{
		py::class_<Image, Resource> imag(handle, "Image");
		py::prepare_for_noncopy_cast<Image>(imag);

		imag.def_property_readonly("width", &Image::GetWidth)
			.def_property_readonly("height", &Image::GetHeight)
			.def_property_readonly("is_filtered", &Image::IsFiltered)

			.def("render", [](const Image& img, const std::vector<Point>& coords, bool mirrorX, const Color& tint) {
				img.Render(coords.data(), mirrorX, tint);
			}, "coords"_a, "mirror_x"_a = false, "tint"_a = Color::White)
			.def("render_part", [](const Image& img, const std::vector<Point>& coords, const std::vector<Point>& texCoords, bool mirrorX, const Color& tint) {
				img.RenderPart(coords.data(), texCoords.data(), mirrorX, tint);
			}, "coords"_a, "tex_coords"_a, "mirror_x"_a = false, "tint"_a = Color::White)

			.def("__repr__", [](const Image& img) { return "<Sharpheus.Image \"" + img.GetPath() + "\" [" + std::to_string(img.GetWidth()) + "x"
				+ std::to_string(img.GetWidth()) + (img.IsFiltered() ? "] filtered>" : "]>"); });
	}


	byte kwargs2FontStyle(const py::kwargs& kwargs)
	{
		byte style = 0;
		if (kwargs.contains("bold") && kwargs["bold"].cast<bool>())
			style |= SPH_FONT_BOLD;
		if (kwargs.contains("italic") && kwargs["italic"].cast<bool>())
			style |= SPH_FONT_ITALIC;
		if (kwargs.contains("underlined") && kwargs["underlined"].cast<bool>())
			style |= SPH_FONT_UNDERLINED;
		return style;
	}

	void Export_Font(py::module_& handle)
	{
		py::class_<Font, Resource> font(handle, "Font");
		py::prepare_for_noncopy_cast<Font>(font);

		font.def_property_readonly("image", &Font::GetImage, py::return_value_policy::reference)
			.def_property_readonly("name", &Font::GetName)

			.def("render", [](const Font& fnt, const std::string& text, const Point& center, float size, const Color& color, const Point& xAxis, const Point& yAxis, const py::kwargs& kwargs) {
				fnt.Render(text, center, size, color, xAxis, yAxis, kwargs2FontStyle(kwargs));
			}, "text"_a, "center"_a, "size"_a, "color"_a = Color::White, "x_axis"_a = Point::Right, "y_axis"_a = Point::Down)

			.def("get_extent", [](const Font& fnt, const std::string& text, float size, const py::kwargs& kwargs) {
				fnt.GetExtent(text, size, kwargs2FontStyle(kwargs));
			}, "text"_a, "size"_a)

			.def("__repr__", [](const Font& fnt) { return "<Sharpheus.Font " + fnt.GetName() + " \"" + fnt.GetImage()->GetPath() + "\">"; });

	}


	void Export_Animation(py::module_& handle)
	{
		py::class_<Animation, Resource> anim(handle, "Animation");
		py::prepare_for_noncopy_cast<Animation>(anim);

		anim.def_property_readonly("name", &Animation::GetName)
			.def_property_readonly("atlas", &Animation::GetAtlas, py::return_value_policy::reference)
			.def_property_readonly("frame_width", &Animation::GetFrameWidth)
			.def_property_readonly("frame_height", &Animation::GetFrameHeight)
			.def_property_readonly("frame_rows", &Animation::GetFrameRows)
			.def_property_readonly("frame_cols", &Animation::GetFrameCols)
			.def_property_readonly("start_frame", &Animation::GetStartFrame)
			.def_property_readonly("end_frame", &Animation::GetEndFrame)
			.def_property_readonly("frame_time", &Animation::GetFrameTime)
			.def_property_readonly("num_of_frames", &Animation::GetNumOfFrames)
			.def_property_readonly("num_of_all_frames", &Animation::GetNumOfAllFrames)
			.def_property_readonly("full_time", &Animation::GetFullTime)

			.def("render", [](const Animation& anim, const std::vector<Point>& coords, float time, bool mirrorX, const Color& tint) {
				anim.Render(coords.data(), time, mirrorX, tint);
			}, "coords"_a, "time"_a, "mirror_x"_a = false, "tint"_a = Color::White)

			.def("__repr__", [](const Animation& anim) { return "<Sharpheus.Animation \"" + anim.GetPath() + "\">"; });
	}


	void Export_TileSet(py::module_& handle)
	{
		py::class_<TileSet, Resource> tile(handle, "TileSet");
		py::prepare_for_noncopy_cast<TileSet>(tile);

		tile.def_property_readonly("name", &TileSet::GetName)
			.def_property_readonly("atlas", &TileSet::GetAtlas, py::return_value_policy::reference)
			.def_property_readonly("frame_width", &TileSet::GetFrameWidth)
			.def_property_readonly("frame_height", &TileSet::GetFrameHeight)
			.def_property_readonly("frame_rows", &TileSet::GetFrameRows)
			.def_property_readonly("frame_cols", &TileSet::GetFrameCols)
			.def_property_readonly("num_of_tiles", &TileSet::GetNumOfTiles)

			.def("render", [](const TileSet& ts, const std::vector<Point>& coords, byte ind, bool mirrorX, const Color& tint) {
				ts.Render(coords.data(), ind, mirrorX, tint);
			}, "coords"_a, "ind"_a, "mirror_x"_a = false, "tint"_a = Color::White)

			.def("__repr__", [](const TileSet& ts) { return "<Sharpheus.TileSet \"" + ts.GetPath() + "\">"; });
	}


	void Export_Audio(py::module_& handle)
	{	
		py::class_<Audio, Resource> audi(handle, "Audio");
		py::prepare_for_noncopy_cast<Audio>(audi);

		audi.def_property_readonly("name", &Audio::GetName)
			.def_property_readonly("length", &Audio::GetLength)

			.def("__repr__", [](const Audio& aud) { return "<Sharpheus.Audio \"" + aud.GetPath() + "\">"; });
	}


	void Export_AudioPlayer_Module(py::module_& handle)
	{
		py::module_ m = handle.def_submodule("AudioPlayer");
		m.doc() = "Audio player module for Sharpheus";

		py::class_<SoLoud::handle>(m, "Handle")
			.def("__repr__", [](SoLoud::handle h) { return "<Sharpheus.AudioPlayer.Handle " + std::to_string(h) + ">"; });

		m.def("play", &AudioPlayer::Play, "audio"_a, "looping"_a = false, "volume"_a = 100, "play_speed"_a = 1.f);
		m.def("stop", &AudioPlayer::Stop, "handle"_a);
		m.def("stop_all", &AudioPlayer::StopAll);

		m.def("pause", &AudioPlayer::Pause, "handle"_a);
		m.def("resume", &AudioPlayer::Resume, "handle"_a);
		m.def("is_paused", &AudioPlayer::IsPaused, "handle"_a);

		m.def("set_volume", &AudioPlayer::SetVolume, "handle"_a, "volume"_a);
		m.def("set_play_speed", &AudioPlayer::SetPlaySpeed, "handle"_a, "speed"_a);
	}


	void Export_ResourceManager_Module(py::module_& handle)
	{
		py::module_ m = handle.def_submodule("ResourceManager");
		m.doc() = "Module for managing assets and resources for Sharpheus";

		m.def("get_assets_root", &ResourceManager::GetAssetsRoot);

		m.def("fullpath2path", &ResourceManager::FullPathToPath, "fullpath"_a);
		m.def("path2fullpath", &ResourceManager::PathToFullPath, "path"_a);

		m.def("get_shader_path", &ResourceManager::GetShaderPath, "fname"_a);

		m.def("get_scripts_root", &ResourceManager::GetScriptsRoot);
		m.def("get_script_path", &ResourceManager::GetScriptPath, "fname"_a);

		m.def("get_font", py::overload_cast<const std::string&, const std::string&>(&ResourceManager::GetFont), "fontfile"_a, "imgfile"_a, py::return_value_policy::reference);
		m.def("get_font", py::overload_cast<const std::string&>(&ResourceManager::GetFont), "name"_a, py::return_value_policy::reference);

		m.def("get_image", &ResourceManager::GetImage, "path"_a, "filtered"_a = false, py::return_value_policy::reference);
		m.def("get_animation", &ResourceManager::GetAnimation, "path"_a, py::return_value_policy::reference);
		m.def("get_tileset", &ResourceManager::GetTileSet, "path"_a, py::return_value_policy::reference);
		m.def("get_audio", &ResourceManager::GetAudio, "path"_a, py::return_value_policy::reference);
	}


	void Export_ResourceManager(py::module_& handle)
	{
		Export_Resource(handle);
		Export_Image(handle);
		Export_Font(handle);
		Export_Animation(handle);
		Export_TileSet(handle);
		Export_Audio(handle);
		Export_AudioPlayer_Module(handle);
		Export_ResourceManager_Module(handle);
	}

}