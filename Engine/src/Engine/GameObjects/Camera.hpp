#pragma once

#include "ShapedGameObject.hpp"


namespace Sharpheus {

	class SPH_EXPORT Camera : public ShapedGameObject
	{
	public:
		Camera(GameObject* parent, const std::string& name);
		virtual ~Camera() = default;
		virtual void CopyFrom(GameObject* other) override;

		virtual Point Project(const Point& pos);
		virtual Point ScreenPosToGamePos(const Point& pos);
		virtual Point GamePosToScreenPos(const Point& pos);

		virtual void Zoom(float scale);
		virtual void ZoomTo(float scale, const Point& keepInPlace);
		virtual void ZoomToScreen(float scale, const Point& keepInPlace);

		virtual void MoveBy(const Point& delta);
		virtual void MoveByScreen(const Point& delta);

		void SetCurrent(bool set = true);
		inline bool IsCurrent();

		inline float GetWidth() { return customWidth == 0.f ? width : customWidth; }
		inline float GetHeight() { return customHeight == 0.f ? height : customHeight; }
		inline void SetCustomRect(float width, float height) { customWidth = width; customHeight = height; SetSizer(width, height); }

		static inline float GetStaticWidth() { return width; }
		static inline float GetStaticHeight() { return height; }
		static inline void SetStaticRect(float _width, float _height) { width = _width; height = _height; }

		virtual float GetXMin();
		virtual float GetXMax();
		virtual float GetYMin();
		virtual float GetYMax();

		virtual bool Load(FileLoader& fl) override;

		void WindowResized(const WindowResizedEvent& e);

		SPH_DECL_GAMEOBJECT(Camera)

	protected:
		float customWidth = 0.f;
		float customHeight = 0.f;
		static float width;
		static float height;
		bool isCurrent = false;

		virtual bool Save(FileSaver& fs) override;

		virtual bool IsSelected(const Point& pos) override { return false; } // Camera cannot be selected by clicking on it
		virtual void RenderSelection() override;
		virtual inline bool IsCurrentCamera() override { return isCurrent; }
	};

}