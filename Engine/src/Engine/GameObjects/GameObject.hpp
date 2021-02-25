#pragma once

#include "Engine/Events/EventListener.hpp"
#include "Engine/EditorInterface/ClassInfo.hpp"
#include "Engine/FileUnits/FileLoader.hpp"
#include "Engine/FileUnits/FileSaver.hpp"

#define SPH_DECL_GAMEOBJECT(type) \
	static ClassInfo classInfo; \
	virtual inline Type GetType() override { return Type::type; } \
	static inline Type GetStaticType() { return Type::type; };


namespace Sharpheus {

	class SPH_EXPORT GameObject : public EventListener
	{
	public:
		enum class Type {
			None,
			Collection,
			Camera,
			Sprite,
			PhysicsObject, BoxCollider,
			Behavior
		};

		GameObject(GameObject* parent, const std::string& name);
		virtual ~GameObject();
		GameObject(const GameObject& other) = default;
		GameObject& operator=(const GameObject& other) = default;

		void					TickAll(float deltaTime);
		void					RenderAll();
		void					RenderAsSelected();

		virtual inline void		SetLevel(class Level* level) { this->level = level; }
		inline GameObject*		GetParent() { return parent; }
		GameObject*				GetRoot();
		void					Move(GameObject* newParent);

		inline Transform&		GetTrafo() { return trafo; }
		inline Transform&		GetWorldTrafo() { return worldTrafo; }
		virtual void			SetTrafo(const Transform& trafo);
		virtual void			SetWorldTrafo(const Transform& trafo);

		GameObject*								GetChild(const std::string& name);
		inline const std::vector<GameObject*>&	GetChildren() { return children; }
		inline const std::string&				GetName() { return name; }
		void									SetName(const std::string& name);

		virtual inline Type		GetType() { return Type::None; }
		static inline Type		GetStaticType() { return Type::None; }

		bool		IsParentOfCurrentCamera();
		GameObject* GetUpperMostSelected(const Point& pos);

		bool SaveAll(FileSaver& fs);
		virtual bool Load(FileLoader& fl);

	protected:
		std::string name;
		Transform trafo;
		Transform worldTrafo;
		
		class Level* level = nullptr;
		GameObject* parent = nullptr;
		std::vector<GameObject*> children;

		static float selectCircleRadius;
		static Color selectColor;

		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);

		virtual bool Save(FileSaver& file);

		virtual bool IsSelected(const Point& pos);

		virtual inline bool IsCurrentCamera() { return false; }

		virtual void Tick(float deltaTime) = 0;
		virtual void Render() = 0;
		virtual void RenderSelection();

		virtual void UpdateWorldTrafo(const Transform& parentWorldTrafo);
	};

}