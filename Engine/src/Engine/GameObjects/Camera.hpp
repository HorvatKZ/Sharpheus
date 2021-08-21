#pragma once

#include "ShapedGameObject.hpp"


namespace Sharpheus {

	class SPH_EXPORT Camera : public ShapedGameObject
	{
	public:
		Camera(GameObject* parent, const std::string& name);
		virtual ~Camera() = default;
		virtual void CopyFrom(GameObject* other) override;

		virtual Point ScreenPosToGamePos(const Point& pos);
		virtual Point GamePosToScreenPos(const Point& pos);

		virtual void Zoom(float scale);
		virtual void ZoomTo(float scale, const Point& keepInPlace);
		virtual void ZoomToScreen(float scale, const Point& keepInPlace);

		virtual void MoveBy(const Point& delta);
		virtual void MoveByScreen(const Point& delta);

		void SetCurrent(bool set = true);
		inline bool IsCurrent();

		virtual inline float GetWidth() { return width; }
		virtual inline float GetHeight() { return height; }
		virtual inline float GetOGHeight() { return ogHeight; }
		virtual inline float GetOGWidth() { return ogHeight * width / height; }

		static inline float GetStaticWidth() { return width; }
		static inline float GetStaticHeight() { return height; }
		static inline void SetStaticRect(float _width, float _height) { width = _width; height = _height; }
		static inline void SetOGHeight(float _height) { ogHeight = _height; }

		virtual float GetXMin();
		virtual float GetXMax();
		virtual float GetYMin();
		virtual float GetYMax();

		virtual inline Shape* GetShape() {
			SetSizer(GetWidth(), GetHeight());
			return shape;
		}

		virtual bool Load(FileLoader& fl) override;

		void WindowResized(const WindowResizedEvent& e);

		SPH_DECL_GAMEOBJECT(Camera)

	protected:
		static float width;
		static float height;
		static float ogHeight;
		bool isCurrent = false;

		virtual bool Save(FileSaver& fs) override;

		virtual bool IsSelected(const Point& pos) override { return false; } // Camera cannot be selected by clicking on it
		virtual void RenderSelection() override;
		virtual inline bool IsCurrentCamera() override { return isCurrent; }
	};

}