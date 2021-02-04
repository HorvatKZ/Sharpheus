#pragma once

#include "CollisionEvent.hpp"


namespace Sharpheus {

	class CollisionSystem
	{
	public:
		static void Init();
		static void Clear();

		static void Tick();

		static void AddCollider(class Collider* collider);
		static void RemoveCollider(class Collider* collider);

		static void ShowColliders();
		static void HideColliders();

	private:
		static std::vector<class Collider*> colliders;
		static bool areCollidersVisible;
	};

}