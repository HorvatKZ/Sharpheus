#pragma once

#include "RectGameObject.hpp"


namespace Sharpheus {

	class SPH_EXPORT Camera : public RectGameObject
	{
	public:
		Camera(GameObject* parent, const std::string& name);
		virtual ~Camera() = default;

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

		static inline float GetWidth() { return width; }
		static inline float GetHeight() { return height; }
		static inline void SetUpRect(float _width, float _height) { width = _width; height = _height; }

		virtual float GetXMin();
		virtual float GetXMax();
		virtual float GetYMin();
		virtual float GetYMax();

		void WindowResized(const WindowResizedEvent& e);

		SPH_DECL_GAMEOBJECT(Camera)

	protected:
		static float width;
		static float height;
		bool isCurrent = false;

		virtual void RecalcOffsets() override;
		virtual void RenderSelection() override;
		virtual inline bool IsCurrentCamera() override { return isCurrent; }

		virtual void Tick(float deltaTime) override;
		virtual void Render() override;
	};

}