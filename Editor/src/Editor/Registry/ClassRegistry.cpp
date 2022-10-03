#include "editor_pch.h"
#include "ClassRegistry.hpp"


namespace Sharpheus {

	std::unordered_map<GameObject::Type, ClassInfoPtr> ClassRegistry::classInfos;

	void ClassRegistry::Init()
	{
		classInfos[GameObject::Type::Collection]				= &Collection::classInfo;
		classInfos[GameObject::Type::Camera]					= &Camera::classInfo;
		classInfos[GameObject::Type::Sprite]					= &Sprite::classInfo;
		classInfos[GameObject::Type::Quad]						= &Quad::classInfo;
		classInfos[GameObject::Type::Text]						= &Text::classInfo;
		classInfos[GameObject::Type::AnimatedSprite]			= &AnimatedSprite::classInfo;
		classInfos[GameObject::Type::AnimationPlayer]			= &AnimationPlayer::classInfo;
		classInfos[GameObject::Type::TileMap]					= &TileMap::classInfo;
		classInfos[GameObject::Type::ParticleEmitter]			= &ParticleEmitter::classInfo;
		classInfos[GameObject::Type::PhysicsObject]				= &PhysicsObject::classInfo;
		classInfos[GameObject::Type::BoxCollider]				= &BoxCollider::classInfo;
		classInfos[GameObject::Type::CircleCollider]			= &CircleCollider::classInfo;
		classInfos[GameObject::Type::CapsuleCollider]			= &CapsuleCollider::classInfo;
		classInfos[GameObject::Type::MusicPlayer]				= &MusicPlayer::classInfo;
		classInfos[GameObject::Type::SoundEffects]				= &SoundEffects::classInfo;
		classInfos[GameObject::Type::Button]					= &Button::classInfo;
		classInfos[GameObject::Type::ImageButton]				= &ImageButton::classInfo;
		classInfos[GameObject::Type::CheckBox]					= &CheckBox::classInfo;
		classInfos[GameObject::Type::RadioButton]				= &RadioButton::classInfo;
	}

	void ClassRegistry::Clear()
	{
	}

	std::unordered_map<GameObject::Type, const std::string&> ClassRegistry::GetAllNames()
	{
		std::unordered_map<GameObject::Type, const std::string&> result;

		for (auto it = classInfos.begin(); it != classInfos.end(); ++it) {
			result.insert({ (*it).first, (*it).second->GetName() });
		}

		return result;
	}

	std::unordered_map<GameObject::Type, const std::string&> ClassRegistry::GetAllIconPaths()
	{
		std::unordered_map<GameObject::Type, const std::string&> result;
		
		for (auto it = classInfos.begin(); it != classInfos.end(); ++it) {
			result.insert({ (*it).first, (*it).second->GetIconPath() });
		}

		return result;
	}
}