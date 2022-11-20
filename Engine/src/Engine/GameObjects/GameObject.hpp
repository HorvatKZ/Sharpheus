#pragma once

#include "Engine/Events/EventListener.hpp"
#include "Engine/Events/LocalEvent.hpp"
#include "Engine/EditorInterface/ClassInfo.hpp"
#include "Engine/FileUtils/FileLoader.hpp"
#include "Engine/FileUtils/FileSaver.hpp"

#define SPH_DECL_GAMEOBJECT(type) \
	static const ClassInfo classInfo; \
	virtual inline Type GetType() override { return Type::type; } \
	static inline Type GetStaticType() { return Type::type; };

#define SPH_CHECKTYPE(variable, type) \
	if (!variable->Is(Type::type)) { \
		SPH_ERROR("\"{0}\" tries to copy from different type \"{1}\"", name, variable->GetName()); \
		return; \
	}

#define SPH_CHECKMASK(variable, mask) \
	if (!variable->Is(TypeMasks::mask)) { \
		SPH_ERROR("\"{0}\" tries to copy from different masked type \"{1}\"", name, variable->GetName()); \
		return; \
	}


namespace Sharpheus {

	class GameObjectDestroyedEvent;
	SPH_DEF_EVENT_FUNC(GameObjectDestroyedEvent)


	class SPH_EXPORT GameObject : public EventListener
	{
	public:
		enum class Type : byte {
			// Basic types
			None = 0x00,
			Collection = 0x01,

			// Cameras
			Camera = 0x10,

			// Renderables
			Sprite = 0x20,
			Quad = 0x21,
			Text = 0x22,
			AnimatedSprite = 0x23,
			AnimationPlayer = 0x24,
			TileMap = 0x25,
			ParticleEmitter = 0x26,
			Circle = 0x27,

			// Basic physics
			PhysicsObject = 0x30,
			
			// Colliders
			BoxCollider = 0x40,
			CircleCollider = 0x41,
			CapsuleCollider = 0x42,

			// Sounds
			MusicPlayer = 0x50,
			SoundEffects = 0x51,

			// Controls
			Button = 0x60,
			ImageButton = 0x61,
			CheckBox = 0x62,
			RadioButton = 0x63,

			CppBehavior = 0x70,
			PythonRunnerBehavior = 0x71,
			PythonBehavior = 0x72
		};

		enum class TypeMasks : byte {
			Basic = 0x00,
			Camera = 0x10,
			Renderables = 0x20,
			Physics = 0x30,
			Collider = 0x40,
			Sounds = 0x50,
			Control = 0x60,
			Behavior = 0x70,

			MASK = 0xF0
		};


		GameObject(GameObject* parent, const std::string& name);
		virtual ~GameObject();
		GameObject(const GameObject& other) = default;
		GameObject& operator=(const GameObject& other) = default;
		GameObject(GameObject&& other);

		virtual void CopyFrom(GameObject* other);

		void					TickAll(float deltaTime);
		virtual void			RenderAsSelected();

		inline class Level*		GetLevel() { return level; }
		virtual inline void		SetLevel(class Level* level) { this->level = level; }
		inline GameObject*		GetParent() { return parent; }
		GameObject*				GetRoot();
		bool					IsDescendantOf(GameObject* other);
		bool					Move(GameObject* newParent);

		static inline bool		IsOfMask(Type type, TypeMasks mask) { return ((byte)type & (byte)TypeMasks::MASK) == (byte)mask; }
		inline bool				Is(Type type)		{ return GetType() == type; }
		inline bool				Is(TypeMasks mask) { return IsOfMask(GetType(), mask); }

		inline const Transform& GetTrafo() { return trafo; }
		inline const Transform&	GetWorldTrafo() { return worldTrafo; }
		virtual void			SetTrafo(const Transform& trafo);
		virtual void			SetWorldTrafo(const Transform& trafo);

		inline bool				IsVisible() { return isVisible; }
		inline void				SetVisible(bool visibility) { isVisible = visibility; }
		inline void				SwitchVisibility() { isVisible = !isVisible; }
		inline bool				IsAllVisible() {
			if (parent == nullptr) {
				return isVisible;
			}
			return isVisible && parent->IsAllVisible();
		}

		GameObject*								GetChild(const std::string& name);
		GameObject*								GetChild(uint32 ind);
		GameObject*								GetLastChild();
		GameObject*								GetFirstChildOfType(Type type);
		GameObject*								GetLastChildOfType(Type type);
		GameObject*								GetFirstChildOfMask(TypeMasks mask);
		GameObject*								GetLastChildOfMask(TypeMasks mask);
		inline const std::vector<GameObject*>&	GetChildren() { return children; }

		inline const std::string&				GetName() { return name; }
		void									SetName(const std::string& name);
		void									SetUpName(const std::string& name);

		virtual inline Type		GetType() = 0;

		inline bool operator==(const GameObject& other) const { return listenerID == other.listenerID; }
		inline bool operator!=(const GameObject& other) const { return listenerID != other.listenerID; }

		bool		IsParentOfCurrentCamera();
		GameObject* GetUpperMostSelected(const Point& pos);

		inline void SubscribeForDestruction(ID id, GameObjectDestroyedEventFunc&& func) {
			onDestroySubscribers[id] = std::move(func);
		}
		inline void UnSubscribeForDestruction(ID id) {
			auto it = onDestroySubscribers.find(id);
			if (it != onDestroySubscribers.end()) onDestroySubscribers.erase(it);
		}
		inline void SubscribeAsSafeObject(ID id, GameObjectDestroyedEventFunc&& func) {
			safeObjects[id] = std::move(func);
		}
		inline void UnSubscribeAsSafeObject(ID id) {
			auto it = safeObjects.find(id);
			if (it != safeObjects.end()) safeObjects.erase(it);
		}
		static inline ID GetNextFreeSafeObjectID() {
			return nextFreeSafeObjectID++;
		}

		inline bool WasStill() { return lastPos == worldTrafo.pos; }
		void UpdateLastPosAll();

		bool SaveAll(FileSaver& fs);
		virtual bool Load(FileLoader& fl);

	protected:
		std::string name;
		Transform trafo;
		Transform worldTrafo;
		bool isVisible = true;
		
		class Level* level = nullptr;
		GameObject* parent = nullptr;
		std::vector<GameObject*> children;
		Point lastPos;

		std::unordered_map<ID, GameObjectDestroyedEventFunc> safeObjects;
		std::unordered_map<ID, GameObjectDestroyedEventFunc> onDestroySubscribers;

		static const float selectCircleRadius;
		static const Color selectColor;
		static ID nextFreeSafeObjectID;

		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);

		void SetChildByName(GameObject* newChild);

		virtual bool Save(FileSaver& file);

		virtual bool IsSelected(const Point& pos);

		virtual inline bool IsCurrentCamera() { return false; }

		virtual void Tick(float deltaTime) {}
		virtual void RenderSelection();

		virtual void UpdateWorldTrafo(const Transform& parentWorldTrafo);
	};


	template <class T>
	class SafeObject
	{
	public:
		SafeObject(T* target = nullptr) : target(target), id(GameObject::GetNextFreeSafeObjectID()) {
			if (target != nullptr) {
				target->SubscribeAsSafeObject(id, SPH_BIND(SafeObject<T>::OnDestroyed));
			}
		}

		virtual ~SafeObject() {
			if (target != nullptr) {
				target->UnSubscribeAsSafeObject(id);
			}
		}

		SafeObject(const SafeObject<T>& other) : target(other.target), id(GameObject::GetNextFreeSafeObjectID()) {
			if (target != nullptr) {
				target->SubscribeAsSafeObject(id, SPH_BIND(SafeObject<T>::OnDestroyed));
			}
		}

		SafeObject<T>& operator=(const SafeObject<T>& other) {
			target = other.target;
			id = GameObject::GetNextFreeSafeObjectID();
			if (target != nullptr) {
				target->SubscribeAsSafeObject(id, SPH_BIND(SafeObject<T>::OnDestroyed));
			}
			return *this;
		}

		SafeObject(SafeObject<T>&& other) : target(other.target), id(other.id) {
			if (target != nullptr) {
				target->SubscribeAsSafeObject(id, SPH_BIND(SafeObject<T>::OnDestroyed));
			}
			other.target = nullptr;
			other.id = 0;
		}

		SafeObject<T>& operator=(SafeObject<T>&& other) {
			target = other.target;
			id = other.id;
			if (target != nullptr) {
				target->SubscribeAsSafeObject(id, SPH_BIND(SafeObject<T>::OnDestroyed));
			}
			other.target = nullptr;
			other.id = 0;
			return *this;
		}

		operator bool() const {
			return target != nullptr;
		}

		inline bool Is() const {
			return target != nullptr;
		}

		inline T* Get() {
			return target;
		}

		inline void Set(T* target) {
			if (this->target != nullptr) {
				this->target->UnSubscribeAsSafeObject(id);
			}
			this->target = target;
			if (target != nullptr) {
				target->SubscribeAsSafeObject(id, SPH_BIND(SafeObject<T>::OnDestroyed));
			}
		}

		T* operator->() {
			return target;
		}

	private:
		T* target;
		ID id;

		void OnDestroyed(const GameObjectDestroyedEvent& e) {
			target = nullptr;
		}
	};


	typedef SafeObject<GameObject> SafeGameObject;


	class SPH_EXPORT GameObjectDestroyedEvent : public LocalEvent<GameObject>
	{
	public:
		GameObjectDestroyedEvent(GameObject* source) : LocalEvent<GameObject>(source) {}
		virtual ~GameObjectDestroyedEvent() = default;

		inline std::string ToStr() const override { return "GameObjectDestroyedEvent " + source->GetName(); }
	};

}