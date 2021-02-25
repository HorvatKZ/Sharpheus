#pragma once

#include "CollisionEvent.hpp"


namespace Sharpheus {

	class SPH_EXPORT CollisionSystem
	{
	public:
		CollisionSystem();
		virtual ~CollisionSystem();

		void Tick();

		void Clear();

		void AddCollider(class Collider* collider);
		void RemoveCollider(class Collider* collider);

		void ShowColliders();
		void HideColliders();
		inline bool AreCollidersVisible() { return areCollidersVisible; }

	private:
		std::vector<class Collider*> colliders;
		bool areCollidersVisible = false;
	};

}