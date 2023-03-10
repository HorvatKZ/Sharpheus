#pragma once

#include "CollisionEvent.hpp"


namespace Sharpheus {

	class SPH_EXPORT CollisionSystem
	{
	public:
		CollisionSystem();
		virtual ~CollisionSystem();

		void Tick();
		inline void SetRoot(GameObject* root) { this->root = root; }

		void Clear();

		void AddCollider(class Collider* collider);
		void RemoveCollider(class Collider* collider);

		void ShowColliders();
		void HideColliders();
		inline bool AreCollidersVisible() { return areCollidersVisible; }

		void Render();

	private:
		std::vector<class Collider*> colliders;
		bool areCollidersVisible = false;
		GameObject* root = nullptr;
	};

}