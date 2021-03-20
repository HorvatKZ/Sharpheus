#include "pch.h"
#include "Camera.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	ClassInfo Camera::classInfo("Camera", "camera.png", {
		new OneWayBoolProvider<Camera>("IsCurrent", SPH_BIND_GETTER(Camera::IsCurrent), SPH_BIND_SETTER(Camera::SetCurrent), true)
	});

	float Camera::width = 0.f;
	float Camera::height = 0.f;


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

		GameObject::CopyFrom(other);
		Camera* trueOther = (Camera*)other;
		customWidth = trueOther->customWidth;
		customHeight = trueOther->customHeight;
		SetSizer(GetWidth(), GetHeight());
	}


	Point Camera::Project(const Point& pos)
	{
		Point diff = (pos - worldTrafo.pos).Rotate(-worldTrafo.rot);
		return Point(2 * diff.x / GetWidth() / worldTrafo.scale.x, -2 * diff.y / GetHeight() / worldTrafo.scale.y);
	}


	Point Camera::ScreenPosToGamePos(const Point& pos)
	{
		Point fromCenter = Point(pos.x - GetWidth() / 2, pos.y - GetHeight() / 2).Rotate(worldTrafo.rot);
		return Point(worldTrafo.pos.x + fromCenter.x * worldTrafo.scale.x, worldTrafo.pos.y + fromCenter.y * worldTrafo.scale.y);
	}


	Point Camera::GamePosToScreenPos(const Point& pos)
	{
		Point diff = (pos - worldTrafo.pos).Rotate(-worldTrafo.rot);
		return Point(diff.x / worldTrafo.scale.x + GetWidth() / 2, diff.y / worldTrafo.scale.y + GetHeight() / 2);
	}


	void Camera::Zoom(float scale)
	{
		worldTrafo.scale *= scale;
		SetWorldTrafo(worldTrafo);
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
		worldTrafo.pos = ScreenPosToGamePos(newScreenCenter);
		worldTrafo.scale *= scale;
		SetWorldTrafo(worldTrafo);
	}


	void Camera::MoveBy(const Point& delta)
	{
		trafo.pos += delta;
		SetTrafo(trafo);
	}


	void Camera::MoveByScreen(const Point& delta)
	{
		trafo.pos += Point(delta.x * trafo.scale.x, delta.y * trafo.scale.y);
		SetTrafo(trafo);
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
		GameObject::Save(fs);
		fs.Write(isCurrent);
		return fs.GetStatus();
	}


	bool Camera::Load(FileLoader& fl)
	{
		GameObject::Load(fl);
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


	void Camera::Tick(float deltaTime) {}


	void Camera::Render() {}

}
