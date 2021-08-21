#pragma once

#include "ShapedGameObject.hpp"


namespace Sharpheus {
	
	class SPH_EXPORT RenderableGameObject : public ShapedGameObject
	{
		friend class Level;
	public:
		RenderableGameObject(GameObject* parent, const std::string& name, Shape* shape);
		virtual ~RenderableGameObject();
		virtual void CopyFrom(GameObject * other) override;

		virtual inline void SetLevel(class Level* level) override { this->level = level; SetLayer("Default"); }
		inline const std::string& GetLayer() { return layer; }
		bool SetLayer(const std::string& layer);

		inline void RenderIfVisible() {
			if (IsAllVisible()) {
				Render();
			}
		}
		virtual void Render() = 0;
		virtual void RenderAsSelected() override;

		virtual bool Load(FileLoader& fl) override;

	protected:
		std::string layer;

		virtual bool Save(FileSaver& fl) override;
		inline void SetLayerByLevel(const std::string& layer) { this->layer = layer; }
	};

}