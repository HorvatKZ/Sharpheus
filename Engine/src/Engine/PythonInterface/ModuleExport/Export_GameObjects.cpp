#include "pch.h"
#include "PydExport.hpp"
#include "Engine/CollisionSystem/CollData.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"
#include "Engine/GameObjects/GameObjects.h"
#include "Engine/Level.hpp"


namespace Sharpheus {

	void Export_GameObject(py::module_& handle)
	{
		py::class_<GameObject, EventListener> gobj(handle, "GameObject");

		py::enum_<GameObject::Type>(gobj, "Type")
			.value("None", GameObject::Type::None)
			.value("Collection", GameObject::Type::Collection)
			.value("CppBehavior", GameObject::Type::CppBehavior)
			.value("PythonRunnerBehavior", GameObject::Type::PythonRunnerBehavior)
			.value("PythonBehavior", GameObject::Type::PythonBehavior)

			.value("Camera", GameObject::Type::Camera)

			.value("Sprite", GameObject::Type::Sprite)
			.value("Quad", GameObject::Type::Quad)
			.value("Text", GameObject::Type::Text)
			.value("AnimatedSprite", GameObject::Type::AnimatedSprite)
			.value("AnimationPlayer", GameObject::Type::AnimationPlayer)
			.value("TileMap", GameObject::Type::TileMap)
			.value("ParticleEmitter", GameObject::Type::ParticleEmitter)

			.value("PhysicsObject", GameObject::Type::PhysicsObject)

			.value("BoxCollider", GameObject::Type::BoxCollider)
			.value("CircleCollider", GameObject::Type::CircleCollider)
			.value("CapsuleCollider", GameObject::Type::CapsuleCollider)

			.value("MusicPlayer", GameObject::Type::MusicPlayer)
			.value("SoundEffects", GameObject::Type::SoundEffects)

			.value("Button", GameObject::Type::Button)
			.value("ImageButton", GameObject::Type::ImageButton)
			.value("CheckBox", GameObject::Type::CheckBox)
			.value("RadioButton", GameObject::Type::RadioButton);

		py::enum_<GameObject::TypeMasks>(gobj, "TypeMasks")
			.value("Basic", GameObject::TypeMasks::Basic)
			.value("Camera", GameObject::TypeMasks::Camera)
			.value("Renderables", GameObject::TypeMasks::Renderables)
			.value("Physics", GameObject::TypeMasks::Physics)
			.value("Collider", GameObject::TypeMasks::Collider)
			.value("Sounds", GameObject::TypeMasks::Sounds)
			.value("Control", GameObject::TypeMasks::Control);

		gobj.def_property("name", &GameObject::GetName, &GameObject::SetName)
			.def_property("trafo", &GameObject::GetTrafo, &GameObject::SetTrafo)
			.def_property("world_trafo", &GameObject::GetWorldTrafo, &GameObject::SetWorldTrafo)
			.def_property("is_visible", &GameObject::IsVisible, &GameObject::SetVisible)
			.def_property_readonly("parent", &GameObject::GetParent, py::return_value_policy::reference)
			.def_property_readonly("root", &GameObject::GetRoot, py::return_value_policy::reference)
			.def_property_readonly("children", &GameObject::GetChildren, py::return_value_policy::reference)
			.def_property_readonly("level", &GameObject::GetLevel, py::return_value_policy::reference)

			.def("copy_from", &GameObject::CopyFrom)

			.def("render_as_selected", &GameObject::RenderAsSelected)

			.def("is_descendant_of", &GameObject::IsDescendantOf, "other"_a)
			.def("move", &GameObject::Move, "new_parent"_a)

			.def("is", py::overload_cast<GameObject::Type>(&GameObject::Is), "type"_a)
			.def("is", py::overload_cast<GameObject::TypeMasks>(&GameObject::Is), "mask"_a)

			.def("switch_visibility", &GameObject::SwitchVisibility)
			.def("is_all_visible", &GameObject::IsAllVisible)

			.def("get_child", py::overload_cast<const std::string&>(&GameObject::GetChild), "name"_a, py::return_value_policy::reference)
			.def("get_child", py::overload_cast<uint32>(&GameObject::GetChild), "ind"_a, py::return_value_policy::reference)
			.def("get_last_child", &GameObject::GetLastChild, py::return_value_policy::reference)
			.def("get_first_child_of_type", &GameObject::GetFirstChildOfType, "type"_a, py::return_value_policy::reference)
			.def("get_last_child_of_type", &GameObject::GetLastChildOfType, "type"_a, py::return_value_policy::reference)
			.def("get_first_child_of_mask", &GameObject::GetFirstChildOfMask, "mask"_a, py::return_value_policy::reference)
			.def("get_last_child_of_mask", &GameObject::GetLastChildOfMask, "mask"_a, py::return_value_policy::reference)

			.def("type", &GameObject::GetType)

			.def(py::self == py::self)
			.def(py::self != py::self)

			.def("is_parent_of_current_camera", &GameObject::IsParentOfCurrentCamera);

		py::class_<Collection>(handle, "Collection", gobj)
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a);
	}


	void Export_GameObjectHierarchy(py::module_& handle)
	{
		py::class_<ShapedGameObject, GameObject>(handle, "ShapedGameObject")
			.def_property_readonly("shape", &ShapedGameObject::GetShape, py::return_value_policy::reference);

		py::class_<RenderableGameObject, ShapedGameObject>(handle, "RenderableGameObject")
			.def_property("layer", &RenderableGameObject::GetLayer, &RenderableGameObject::SetLayer)
			.def("render_if_visible", &RenderableGameObject::RenderIfVisible)
			.def("render", &RenderableGameObject::Render);

		py::class_<AxisGameObject, RenderableGameObject>(handle, "AxisGameObject")
			.def_property_readonly("x_axis", &AxisGameObject::GetXAxis)
			.def_property_readonly("y_axis", &AxisGameObject::GetYAxis);
	}


	void Export_Behaviors(py::module_& handle)
	{
		py::class_<Behavior, GameObject>(handle, "Behavior")
			.def("init", &Behavior::Init)
			.def("sub_type", &Behavior::GetSubType);

		py::class_<PythonRunnerBehavior, Behavior>(handle, "PythonRunnerBehavior")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a);

		py::class_<PythonBehavior, Behavior, PythonBehaviorTrampoline>(handle, "PythonBehavior")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def("subscribe_for_render", [](PythonBehavior& self, const std::string& layerName, std::function<void()>&& func) {
				self.SubscribeForRender(self.GetLevel(), layerName, std::move(func));
				}, "layer_name"_a, "func"_a)
			.def("unsubscribe_for_render", &PythonBehavior::UnSubscribeForRender)
			.def("subscribe_collision", &PythonBehavior::SubscribeCollision)
			.def("unsubscribe_collision", &PythonBehavior::UnSubscribeCollision)
			.def("subscribe_trigger_enter", &PythonBehavior::SubscribeOnTriggerEnter)
			.def("unsubscribe_trigger_enter", &PythonBehavior::SubscribeOnTriggerEnter)
			.def("subscribe_trigger_exit", &PythonBehavior::SubscribeOnTriggerExit)
			.def("unsubscribe_trigger_exit", &PythonBehavior::SubscribeOnTriggerExit)
			.def("subscribe_control_changed", &PythonBehavior::SubscribeControlChanged)
			.def("unsubscribe_control_changed", &PythonBehavior::UnSubscribeControlChanged);
	}


	void Export_Colliders(py::module_& handle)
	{
		py::class_<CollData>(handle, "CollData")
			.def(py::init<const Shape::Intersection&, bool, const Point&, float>(), "geom"_a, "is_dynamic"_a, "v_other"_a, "m_other"_a)
			.def_readonly("geom", &CollData::geom)
			.def_readonly("is_dynamic", &CollData::isDynamic)
			.def_readonly("v_other", &CollData::vOther)
			.def_readonly("m_other", &CollData::mOther)
			.def_property_readonly("is", &CollData::Is);

		py::class_<CollisionEvent, Event>(handle, "CollisionEvent")
			.def(py::init<Collider*, Collider*, const CollData&>(), "source"_a, "with"_a, "cd"_a)
			.def_readonly("source", &CollisionEvent::source)
			.def_readonly("with", &CollisionEvent::with)
			.def_readonly("cd", &CollisionEvent::cd);
		
		py::class_<OnEnterEvent, Event>(handle, "OnEnterEvent")
			.def(py::init<Collider*, GameObject*>(), "source"_a, "obj"_a)
			.def_readonly("source", &OnEnterEvent::source)
			.def_readonly("obj", &OnEnterEvent::obj);

		py::class_<OnExitEvent, Event>(handle, "OnExitEvent")
			.def(py::init<Collider*, GameObject*, bool>(), "source"_a, "obj"_a, "obj_destroyed"_a)
			.def_readonly("source", &OnExitEvent::source)
			.def_readonly("obj", &OnExitEvent::obj)
			.def_readonly("obj_destroyed", &OnExitEvent::objDestroyed);

		py::class_<Collider, ShapedGameObject>(handle, "Collider")
			.def_property("is_trigger", &Collider::IsTrigger, &Collider::SetTrigger)
			.def_property_readonly("is_dynamic", &Collider::IsDynamic)
			.def_property_readonly("is_collider_visible", &Collider::IsColliderVisible);

		py::class_<BoxCollider, Collider>(handle, "BoxCollider")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("width", &BoxCollider::GetWidth, &BoxCollider::SetWidth)
			.def_property("height", &BoxCollider::GetHeight, &BoxCollider::SetHeight);

		py::class_<CircleCollider, Collider>(handle, "CircleCollider")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("radius", &CircleCollider::GetRadius, &CircleCollider::SetRadius);

		py::class_<CapsuleCollider, Collider>(handle, "CapsuleCollider")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("width", &CapsuleCollider::GetWidth, &CapsuleCollider::SetWidth)
			.def_property("height", &CapsuleCollider::GetHeight, &CapsuleCollider::SetHeight);
	}


	void Export_PhysicsObject(py::module_& handle)
	{
		py::class_<PhysicsObject, GameObject>(handle, "PhysicsObject")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("velocity", &PhysicsObject::GetVelocity, &PhysicsObject::SetVelocity)
			.def_property("mass", &PhysicsObject::GetMass, &PhysicsObject::SetMass)
			.def_property("gravity", &PhysicsObject::GetGravity, &PhysicsObject::SetGravity)
			.def_property("bounce", &PhysicsObject::GetBounce, &PhysicsObject::SetBounce)
			.def_property("friction", &PhysicsObject::GetFriction, &PhysicsObject::SetFriction)
			.def_property("friction", &PhysicsObject::GetFriction, &PhysicsObject::SetFriction)
			.def("add_velocity", &PhysicsObject::AddVelocity, "velocity"_a);
	}


	void Export_Controls(py::module_& handle)
	{
		py::class_<ControlChangedEvent, Event>(handle, "ControlChangedEvent")
			.def(py::init<Control*>(), "source"_a)
			.def_readonly("source", &ControlChangedEvent::source);

		py::class_<Control, AxisGameObject>(handle, "Control")
			.def_property("text", &Control::GetText, &Control::SetText)
			.def_property("font", &Control::GetFont, &Control::SetFont, py::return_value_policy::reference)
			.def_property("font_color", &Control::GetFontColor, &Control::SetFontColor)
			.def_property("font_size", &Control::GetFontSize, &Control::SetFontSize)
			.def_property("font_style", &Control::GetFontStyle, &Control::SetFontStyle)
			.def_property_readonly("state", &Control::GetState)
			.def_property_readonly("is_clicked", &Control::IsCurrentlyClicked)

			.def("set_font_by_name", &Control::SetFontByName, "name"_a)
			.def("set_font_by_name", &Control::SetFontByPath, "fontfile"_a, "imgfile"_a)

			.def("set_font_style", [](Control& ctrl, const py::kwargs& kwargs) {
				ctrl.SetFontStyle(kwargs2FontStyle(kwargs));
			})
			
			.def("subscribe_for_change", &Control::SubscribeForChange, "subscriber_id"_a, "func"_a)
			.def("unsubscribe_for_change", &Control::UnSubscribeForChange, "subscriber_id"_a);

		py::class_<ButtonBase, Control>(handle, "ButtonBase")
			.def_property("color", &ButtonBase::GetColor, &ButtonBase::SetColor)
			.def_property("clicked_color", &ButtonBase::GetClickedColor, &ButtonBase::SetClickedColor)
			.def_property("clicked_font_color", &ButtonBase::GetClickedFontColor, &ButtonBase::SetClickedFontColor)
			.def_property("use_clicked_colors", &ButtonBase::DoesUseClickedColors, &ButtonBase::SetUseClickedColors)
			.def_property_readonly("click_num", &ButtonBase::GetClickNum);

		py::class_<Button, ButtonBase>(handle, "Button")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("width", &Button::GetWidth, &Button::SetWidth)
			.def_property("height", &Button::GetHeight, &Button::SetHeight);

		py::class_<ImageButton, ButtonBase>(handle, "ImageButton")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("img", &ImageButton::GetImg, &ImageButton::SetImg, py::return_value_policy::reference)
			.def_property("clicked_img", &ImageButton::GetClickedImg, &ImageButton::SetClickedImg, py::return_value_policy::reference)

			.def("set_img_from_path", &ImageButton::SetImgFromPath, "path"_a, "filtered"_a)
			.def("set_clicked_img_from_path", &ImageButton::SetClickedImgFromPath, "path"_a, "filtered"_a);

		py::class_<CheckBase, Control>(handle, "CheckBase")
			.def_property("checked_img", &CheckBase::GetCheckedImg, &CheckBase::SetCheckedImg, py::return_value_policy::reference)
			.def_property("unchecked_img", &CheckBase::GetUncheckedImg, &CheckBase::SetUncheckedImg, py::return_value_policy::reference)
			.def_property("tint", &CheckBase::GetTint, &CheckBase::SetTint)
			.def_property("is_text_on_right", &CheckBase::IsTextOnRight, &CheckBase::SetTextOnRight)
			.def_property_readonly("is_checked", &CheckBase::IsChecked)

			.def("set_checked_img_from_path", &CheckBase::SetCheckedImgFromPath, "path"_a, "filtered"_a)
			.def("set_unchecked_img_from_path", &CheckBase::SetUncheckedImgFromPath, "path"_a, "filtered"_a);

		py::class_<CheckBox, CheckBase>(handle, "CheckBox")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a);

		py::class_<RadioButton, CheckBase>(handle, "RadioButton")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("group_id", &RadioButton::GetGroupId, &RadioButton::SetGroupId)
			.def_property("value", &RadioButton::GetValue, &RadioButton::SetValue)

			.def_static("get_value_of", &RadioButton::GetValueOf, "id"_a);
	}


	void Export_Renderables(py::module_& handle)
	{
		py::class_<Sprite, RenderableGameObject>(handle, "Sprite")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("image", &Sprite::GetImage, &Sprite::SetImage, py::return_value_policy::reference)
			.def_property("tint", &Sprite::GetTint, &Sprite::SetTint)
			.def("set_image_from_path", &Sprite::SetImageFromPath, "path"_a, "filtered"_a);

		py::class_<Quad, RenderableGameObject>(handle, "Quad")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("width", &Quad::GetWidth, &Quad::SetWidth)
			.def_property("height", &Quad::GetHeight, &Quad::SetHeight)
			.def_property("color", &Quad::GetColor, &Quad::SetColor);

		py::class_<Circle, RenderableGameObject>(handle, "Circle")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("radius", &Circle::GetRadius, &Circle::SetRadius)
			.def_property("color", &Circle::GetColor, &Circle::SetColor);

		py::class_<Text, RenderableGameObject>(handle, "Text")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("content", &Text::GetContent, &Text::SetContent)
			.def_property("font", &Text::GetFont, &Text::SetFont, py::return_value_policy::reference)
			.def_property("color", &Text::GetColor, &Text::SetColor)
			.def_property("size", &Text::GetSize, &Text::SetSize)
			.def_property("style", &Text::GetStyle, &Text::SetStyle)
			.def_property("is_bold", &Text::IsBold, &Text::SetBold)
			.def_property("is_italic", &Text::IsItalic, &Text::SetItalic)
			.def_property("is_underlined", &Text::IsUnderlined, &Text::SetUnderlined)

			.def("set_font_by_name", &Text::SetFontByName, "name"_a)
			.def("set_font_by_name", &Text::SetFontByPath, "fontfile"_a, "imgfile"_a)

			.def("set_style", [](Text& txt, const py::kwargs& kwargs) {
				txt.SetStyle(kwargs2FontStyle(kwargs));
			});

		py::class_<AnimatedSprite, RenderableGameObject>(handle, "AnimatedSprite")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("animation", &AnimatedSprite::GetAnimation, &AnimatedSprite::SetAnimation, py::return_value_policy::reference)
			.def_property("tint", &AnimatedSprite::GetTint, &AnimatedSprite::SetTint)
			.def_property("speed", &AnimatedSprite::GetSpeed, &AnimatedSprite::SetSpeed)

			.def("set_animation_from_path", &AnimatedSprite::SetAnimationFromPath, "path"_a);

		py::class_<AnimationPlayer, RenderableGameObject>(handle, "AnimationPlayer")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("tint", &AnimationPlayer::GetTint, &AnimationPlayer::SetTint)
			.def_property("speed", &AnimationPlayer::GetSpeed, &AnimationPlayer::SetSpeed)
			.def_property("current", &AnimationPlayer::GetCurrent, &AnimationPlayer::SetCurrent)
			.def_property("current_name", &AnimationPlayer::GetCurrentAnimationName, &AnimationPlayer::SetCurrentByName)
			.def_property_readonly("current_animation", &AnimationPlayer::GetCurrentAnimation, py::return_value_policy::reference)

			.def("get_animation_count", &AnimationPlayer::GetAnimationCount)
			.def("get_animation", &AnimationPlayer::GetAnimation, "ind"_a, py::return_value_policy::reference)
			.def("get_animation_name", &AnimationPlayer::GetAnimationName, "ind"_a)
			.def("set_animation", &AnimationPlayer::SetAnimation, "ind"_a, "anim"_a)
			.def("add_animation", &AnimationPlayer::AddAnimation, "anim"_a)
			.def("remove_animation_by_index", &AnimationPlayer::RemoveAnimationByIndex, "ind"_a)
			.def("remove_animation", &AnimationPlayer::RemoveAnimation, "anim"_a)

			.def("set_animation_from_path", &AnimationPlayer::SetAnimationFromPath, "ind"_a, "path"_a)
			.def("add_animation_from_path", &AnimationPlayer::AddAnimationFromPath, "path"_a)

			.def("play_once", py::overload_cast<uint32>(&AnimationPlayer::PlayOnce), "ind"_a)
			.def("play_once", py::overload_cast<const std::string&>(&AnimationPlayer::PlayOnce), "name"_a);

		py::class_<TileMap, AxisGameObject> tlmp(handle, "TileMap");

		py::class_<TileMap::IntPoint>(tlmp, "IntPoint")
			.def_readwrite("x", &TileMap::IntPoint::x)
			.def_readwrite("y", &TileMap::IntPoint::y)

			.def(py::init<int32, int32>(), "x"_a, "y"_a)

			.def(py::self == py::self)

			.def("to_chunk", &TileMap::IntPoint::ToChunk)
			.def("to_rel_pos", &TileMap::IntPoint::ToRelPos)
			.def("to_rel_ind", &TileMap::IntPoint::ToRelInd)

			.def_static("div_by_chunk_size", &TileMap::IntPoint::DivByChunkSize, "a"_a)
			.def_static("mod_by_chunk_size", &TileMap::IntPoint::ModByChunkSize, "a"_a);

		tlmp.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("tile_set", &TileMap::GetTileSet, &TileMap::SetTileSet, py::return_value_policy::reference)
			.def_property("tint", &TileMap::GetTint, &TileMap::SetTint)

			.def("set_tile_set_from_path", &TileMap::SetTileSetFromPath, "path"_a)

			.def("to_tile_coord", &TileMap::ToTileCoord, "pos"_a)
			.def("get_tile", &TileMap::GetTile, "coord"_a)
			.def("set_tile", &TileMap::SetTile, "coord"_a, "value"_a)
			.def("clear_tile", &TileMap::ClearTile, "coord"_a);

		py::class_<ParticleEmitter, AxisGameObject> part(handle, "ParticleEmitter");

		py::class_<ParticleEmitter::ParticleState>(part, "ParticleState")
			.def_readwrite("direction", &ParticleEmitter::ParticleState::direction)
			.def_readwrite("pos", &ParticleEmitter::ParticleState::pos)
			.def_readwrite("start_time", &ParticleEmitter::ParticleState::startTime)
			.def_readwrite("speed", &ParticleEmitter::ParticleState::speed)

			.def(py::init<const Point&, const Point&, float, float>(), "direction"_a, "pos"_a, "start_time"_a, "speed"_a);

		part.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("particle", &ParticleEmitter::GetParticle, &ParticleEmitter::SetParticle, py::return_value_policy::reference)
			.def_property("particle_scale", &ParticleEmitter::GetParticleScale, &ParticleEmitter::SetParticleScale)
			.def_property("tint", &ParticleEmitter::GetTint, &ParticleEmitter::SetTint)
			.def_property("is_shrink", &ParticleEmitter::IsShrink, &ParticleEmitter::SetShrink)
			.def_property("is_fade", &ParticleEmitter::IsFade, &ParticleEmitter::SetFade)
			.def_property("is_burst", &ParticleEmitter::IsBurst, &ParticleEmitter::SetBurst)
			.def_property("burst_n", &ParticleEmitter::GetBurstN, &ParticleEmitter::SetBurstN)
			.def_property("lifetime", &ParticleEmitter::GetLifeTime, &ParticleEmitter::SetLifeTime)
			.def_property("speed", &ParticleEmitter::GetSpeed, &ParticleEmitter::SetSpeed)
			.def_property("emission_time", &ParticleEmitter::GetEmissionTime, &ParticleEmitter::SetEmissionTime)
			.def_property("gravity", &ParticleEmitter::GetGravity, &ParticleEmitter::SetGravity)
			.def_property("friction", &ParticleEmitter::GetFriction, &ParticleEmitter::SetFriction)

			.def("set_particle_from_path", &ParticleEmitter::SetParticleByPath, "path"_a, "filtered"_a)

			.def("set_custom_func", &ParticleEmitter::SetCustomFunc, "customFunc"_a)
			.def("clear_custom_func", &ParticleEmitter::ClearCustomFunc);
	}


	void Export_Sounds(py::module_& handle)
	{
		py::class_<MusicPlayer, GameObject>(handle, "MusicPlayer")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("is_looping", &MusicPlayer::IsLooping, &MusicPlayer::SetLooping)
			.def_property("starts_by_default", &MusicPlayer::DoesStartByDefault, &MusicPlayer::SetStartByDefault)
			.def_property("volume", &MusicPlayer::GetVolume, &MusicPlayer::SetVolume)
			.def_property("play_speed", &MusicPlayer::GetPlaySpeed, &MusicPlayer::SetPlaySpeed)
			.def_property("music", &MusicPlayer::GetMusic, &MusicPlayer::SetMusic, py::return_value_policy::reference)
			.def_property_readonly("is_playing", &MusicPlayer::IsPlaying)

			.def("set_music_from_path", &MusicPlayer::SetMusicFromPath, "musicPath"_a)

			.def("play", &MusicPlayer::Play)
			.def("stop", &MusicPlayer::Stop)
			.def("pause", &MusicPlayer::Pause)
			.def("resume", &MusicPlayer::Resume);

		py::class_<SoundEffects, GameObject>(handle, "SoundEffects")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def("get_num_of_audios", &SoundEffects::GetNumOfAudios)
			.def("get_audio", &SoundEffects::GetAudio, "ind"_a, py::return_value_policy::reference)
			.def("get_audio_name", &SoundEffects::GetAudioName, "ind"_a)
			.def("set_audio", &SoundEffects::SetAudio, "ind"_a, "audio"_a)
			.def("add_audio", &SoundEffects::AddAudio, "audio"_a)
			.def("remove_audio_by_ind", &SoundEffects::RemoveAudioByInd, "ind"_a)

			.def("set_audio_from_path", &SoundEffects::SetAudioFromPath, "ind"_a, "path"_a)
			.def("add_audio_from_path", &SoundEffects::AddAudioFromPath, "path"_a)
			.def("remove_audio", &SoundEffects::RemoveAudio, "audio"_a)

			.def("play", py::overload_cast<uint32, uint32, float>(&SoundEffects::Play), "ind"_a, "volume"_a = 100, "playSpeed"_a = 1.f)
			.def("play", py::overload_cast<const std::string&, uint32, float>(&SoundEffects::Play), "name"_a, "volume"_a = 100, "playSpeed"_a = 1.f)
			.def("stop_all", &SoundEffects::StopAll);
	}


	void Export_Camera(py::module_& handle)
	{
		py::class_<Camera, ShapedGameObject>(handle, "Camera")
			.def(py::init<GameObject*, const std::string&>(), "parent"_a, "name"_a)
			.def_property("is_current", &Camera::IsCurrent, &Camera::SetCurrent)
			.def_property_readonly("width", &Camera::GetWidth)
			.def_property_readonly("height", &Camera::GetHeight)
			.def_property_readonly("original_width", &Camera::GetOGWidth)
			.def_property_readonly("original_height", &Camera::GetOGHeight)
			.def_property_readonly_static("static_width", &Camera::GetStaticWidth)
			.def_property_readonly_static("static_height", &Camera::GetStaticHeight)
			.def_property_readonly("x_min", &Camera::GetXMin)
			.def_property_readonly("x_max", &Camera::GetXMax)
			.def_property_readonly("y_min", &Camera::GetYMin)
			.def_property_readonly("y_max", &Camera::GetYMax)
			.def_property_readonly("shape", &Camera::GetShape, py::return_value_policy::reference)

			.def("screenpos2gamepos", &Camera::ScreenPosToGamePos, "pos"_a)
			.def("gamepos2screenpos", &Camera::GamePosToScreenPos, "pos"_a)

			.def("zoom", &Camera::Zoom, "scale"_a)
			.def("zoom_to", &Camera::ZoomTo, "scale"_a, "keep_in_place"_a)
			.def("zoom_to_screen", &Camera::ZoomToScreen, "scale"_a, "keep_in_place"_a)

			.def("move_by", &Camera::MoveBy, "delta"_a)
			.def("move_by_screen", &Camera::MoveByScreen, "delta"_a);	
	}


	void Export_GameObjects(py::module_& handle)
	{
		Export_GameObject(handle);
		Export_GameObjectHierarchy(handle);
		Export_Behaviors(handle);
		Export_Colliders(handle);
		Export_PhysicsObject(handle);
		Export_Controls(handle);
		Export_Renderables(handle);
		Export_Sounds(handle);
		Export_Camera(handle);
	}

}