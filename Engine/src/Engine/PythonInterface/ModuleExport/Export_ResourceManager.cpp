#include "pch.h"
#include "PydExport.hpp"
#include "Engine/ResourceManager/AudioPlayer.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	void Export_Resource(py::module_& handle)
	{
		py::class_<Resource>(handle, "Resource")
			.def("is_valid", &Resource::IsValid)
			.def("has_path", &Resource::HasPath)
			.def("get_path", &Resource::GetPath)
			.def("get_full_path", &Resource::GetFullPath);
	}


	void Export_Image(py::module_& handle)
	{
		py::class_<Image, Resource>(handle, "Image")
			.def("get_width", &Image::GetWidth)
			.def("get_height", &Image::GetHeight)
			.def("is_filtered", &Image::IsFiltered)

			.def("render", [](const Image& img, const std::vector<Point>& coords, const Color& tint) {
				img.Render(coords.data(), tint);
			}, "coords"_a, "tint"_a = Color::White)
			.def("render_part", [](const Image& img, const std::vector<Point>& coords, const std::vector<Point>& texCoords, const Color& tint) {
				img.RenderPart(coords.data(), texCoords.data(), tint);
			}, "coords"_a, "tex_coords"_a, "tint"_a = Color::White)

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
		py::class_<Font, Resource>(handle, "Font")
			.def("get_image", &Font::GetImage, py::return_value_policy::reference)
			.def("get_name", &Font::GetName)

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
		py::class_<Animation, Resource>(handle, "Animation")
			.def("get_name", &Animation::GetName)
			.def("get_atlas", &Animation::GetAtlas, py::return_value_policy::reference)
			.def("get_frame_width", &Animation::GetFrameWidth)
			.def("get_frame_height", &Animation::GetFrameHeight)
			.def("get_frame_rows", &Animation::GetFrameRows)
			.def("get_frame_cols", &Animation::GetFrameCols)
			.def("get_start_frame", &Animation::GetStartFrame)
			.def("get_end_frame", &Animation::GetEndFrame)
			.def("get_frame_time", &Animation::GetFrameTime)
			.def("get_num_of_frames", &Animation::GetNumOfFrames)
			.def("get_num_of_all_frames", &Animation::GetNumOfAllFrames)
			.def("get_full_time", &Animation::GetFullTime)

			.def("render", [](const Animation& anim, const std::vector<Point>& coords, float time, const Color& tint) {
				anim.Render(coords.data(), time, tint);
			}, "coords"_a, "time"_a, "tint"_a = Color::White)

			.def("__repr__", [](const Animation& anim) { return "<Sharpheus.Animation \"" + anim.GetPath() + "\">"; });
	}


	void Export_TileSet(py::module_& handle)
	{
		py::class_<TileSet, Resource>(handle, "TileSet")
			.def("get_name", &TileSet::GetName)
			.def("get_atlas", &TileSet::GetAtlas, py::return_value_policy::reference)
			.def("get_frame_width", &TileSet::GetFrameWidth)
			.def("get_frame_height", &TileSet::GetFrameHeight)
			.def("get_frame_rows", &TileSet::GetFrameRows)
			.def("get_frame_cols", &TileSet::GetFrameCols)
			.def("get_num_of_tiles", &TileSet::GetNumOfTiles)

			.def("render", [](const TileSet& ts, const std::vector<Point>& coords, byte ind, const Color& tint) {
				ts.Render(coords.data(), ind, tint);
			}, "coords"_a, "ind"_a, "tint"_a = Color::White)

			.def("__repr__", [](const TileSet& ts) { return "<Sharpheus.TileSet \"" + ts.GetPath() + "\">"; });
	}


	void Export_Audio(py::module_& handle)
	{	
		py::class_<Audio, Resource>(handle, "Audio")
			.def("get_name", &Audio::GetName)
			.def("get_length", &Audio::GetLength)

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