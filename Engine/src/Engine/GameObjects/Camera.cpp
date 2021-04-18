#include "pch.h"
#include "Camera.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	ClassInfo Camera::classInfo("Camera", "camera.png", {
		new OneWayBoolProvider<Camera>("IsCurrent", SPH_BIND_GETTER(Camera::IsCurrent), SPH_BIND_SETTER(Camera::SetCurrent), true)
	});

	float Camera::width = 0.f;
	float Camera::height = 0.f;
	float Camera::ogHeight = 0.f;


	Camera::Camera(GameObject* parent, const std::string& name) :
		ShapedGameObject(parent, name, new Rect())
	{
		SetSizer(width, height);
		shape->ForceRefresh();

		Subscribe<WindowResizedEvent>(SPH_BIND(Camera::WindowResized));
	}


	void Camera::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, Camera);

		ShapedGameObject::CopyFrom(other);
		Camera* trueOther = (Camera*)other;
		SetSizer(GetWidth(), GetHeight());
	}


	Point Camera::ScreenPosToGamePos(const Point& pos)
	{
		Point ogPos = pos * GetOGWidth() / GetWidth();
		Point fromCenter = Point((ogPos.x - GetOGWidth() / 2) * worldTrafo.scale.x, (ogPos.y - GetOGHeight() / 2) * worldTrafo.scale.y).Rotate(worldTrafo.rot);
		return Point(worldTrafo.pos.x + fromCenter.x, worldTrafo.pos.y + fromCenter.y);
	}


	Point Camera::GamePosToScreenPos(const Point& pos)
	{
		Point diff = (pos - worldTrafo.pos).Rotate(-worldTrafo.rot);
		Point ogPos(diff.x / worldTrafo.scale.x + GetOGWidth() / 2, diff.y / worldTrafo.scale.y + GetOGHeight() / 2);
		return ogPos * GetWidth() / GetOGWidth();
	}


	void Camera::Zoom(float scale)
	{
		Transform newTrafo = worldTrafo;
		newTrafo.scale *= scale;
		SetWorldTrafo(newTrafo);
	}


	void Camera::ZoomTo(float scale, const Point& keepInPlace)
	{
		ZoomToScreen(scale, GamePosToScreenPos(keepInPlace));
	}


	void Camera::ZoomToScreen(float scale, const Point& keepInPlace)
	{
		Point diff(keepInPlace.x - GetWidth() / 2, keepInPlace.y - GetHeight() / 2);
		Point newScreenCenterDiff = (1.f - scale) * diff;
		Point newScreenCenter(newScreenCenterDiff.x + GetWidth() / 2, newScreenCenterDiff.y + GetHeight() / 2);
		Transform newTrafo = worldTrafo;
		newTrafo.pos = ScreenPosToGamePos(newScreenCenter);
		newTrafo.scale *= scale;
		SetWorldTrafo(newTrafo);
	}


	void Camera::MoveBy(const Point& delta)
	{
		Transform newTrafo = trafo;
		newTrafo.pos += delta;
		SetTrafo(newTrafo);
	}


	void Camera::MoveByScreen(const Point& delta)
	{
		Transform newTrafo = trafo;
		newTrafo.pos += Point(delta.x * trafo.scale.x, delta.y * trafo.scale.y);
		SetTrafo(newTrafo);
	}


	void Camera::SetCurrent(bool set /*= true*/)
	{
		isCurrent = set;
		if (set) {
			Renderer::SetCamera(this);
		}
	}


	inline bool Camera::IsCurrent()
	{
		return isCurrent;
	}


	bool Camera::Save(FileSaver& fs)
	{
		ShapedGameObject::Save(fs);
		fs.Write(isCurrent);
		return fs.GetStatus();
	}


	bool Camera::Load(FileLoader& fl)
	{
		ShapedGameObject::Load(fl);
		fl.Read(isCurrent);
		if (isCurrent) {
			SetCurrent();
		}
		SetSizer(GetWidth(), GetHeight());
		return fl.GetStatus();
	}


	void Camera::RenderSelection()
	{
		SetSizer(GetWidth(), GetHeight());
		ShapedGameObject::RenderSelection();
	}


	float Camera::GetXMin()
	{
		SetSizer(GetWidth(), GetHeight());
		Point* corners = shape->GetCorners();

		float minX = corners[0].x;
		for (uint32_t i = 1; i < 4; ++i) {
			if (corners[i].x < minX) {
				minX = corners[i].x;
			}
		}
		return minX;
	}


	float Camera::GetXMax()
	{
		SetSizer(GetWidth(), GetHeight());
		Point* corners = shape->GetCorners();

		float maxX = corners[0].x;
		for (uint32_t i = 1; i < 4; ++i) {
			if (corners[i].x > maxX) {
				maxX = corners[i].x;
			}
		}
		return maxX;
	}


	float Camera::GetYMin()
	{
		SetSizer(GetWidth(), GetHeight());
		Point* corners = shape->GetCorners();

		float minY = corners[0].y;
		for (uint32_t i = 1; i < 4; ++i) {
			if (corners[i].y < minY) {
				minY = corners[i].y;
			}
		}
		return minY;
	}


	float Camera::GetYMax()
	{
		SetSizer(GetWidth(), GetHeight());
		Point* corners = shape->GetCorners();

		float maxY = corners[0].y;
		for (uint32_t i = 1; i < 4; ++i) {
			if (corners[i].y > maxY) {
				maxY = corners[i].y;
			}
		}
		return maxY;
	}


	void Camera::WindowResized(const WindowResizedEvent& e)
	{
		width = e.newWidth;
		height = e.newHeight;
		SetSizer(GetWidth(), GetHeight());
	}

}
