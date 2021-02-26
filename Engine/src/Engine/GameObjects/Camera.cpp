#include "pch.h"
#include "Camera.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	ClassInfo Camera::classInfo("Camera", "camera.png", {
		new OneWayBoolProvider<Camera>("IsCurrent", SPH_BIND_GETTER(Camera::IsCurrent), SPH_BIND_SETTER(Camera::SetCurrent), true)
	});

	float Camera::width = 0.f;
	float Camera::height = 0.f;


	SPH_IMPL_COPY(Camera);

	Camera::Camera(GameObject* parent, const std::string& name) :
		RectGameObject(parent, name)
	{
		Subscribe<WindowResizedEvent>(SPH_BIND(Camera::WindowResized));
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
		needToRecalcOffset = true;
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
		needToRecalcOffset = true;
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
		needToRecalcOffset = true;
		return fl.GetStatus();
	}


	void Camera::RecalcOffsets()
	{
		RecalcOffsetsCommon(GetWidth() * worldTrafo.scale.x, GetHeight() * worldTrafo.scale.y, worldTrafo.rot);
	}

	void Camera::RenderSelection()
	{
		if (needToRecalcOffset) {
			RecalcOffsets();
		}

		RectGameObject::RenderSelection();
	}


	float Camera::GetXMin()
	{
		if (needToRecalcOffset) {
			RecalcOffsets();
		}

		float minX = worldTrafo.pos.x + offsets[0].x;
		for (uint32_t i = 1; i < 4; ++i) {
			if (offsets[i].x < minX) {
				minX = worldTrafo.pos.x + offsets[i].x;
			}
		}
		return minX;
	}


	float Camera::GetXMax()
	{
		if (needToRecalcOffset) {
			RecalcOffsets();
		}

		float maxX = worldTrafo.pos.x + offsets[0].x;
		for (uint32_t i = 1; i < 4; ++i) {
			if (offsets[i].x > maxX) {
				maxX = worldTrafo.pos.x + offsets[i].x;
			}
		}
		return maxX;
	}


	float Camera::GetYMin()
	{
		if (needToRecalcOffset) {
			RecalcOffsets();
		}

		float minY = worldTrafo.pos.y + offsets[0].y;
		for (uint32_t i = 1; i < 4; ++i) {
			if (offsets[i].y < minY) {
				minY = worldTrafo.pos.y + offsets[i].y;
			}
		}
		return minY;
	}


	float Camera::GetYMax()
	{
		if (needToRecalcOffset) {
			RecalcOffsets();
		}

		float maxY = worldTrafo.pos.y + offsets[0].y;
		for (uint32_t i = 1; i < 4; ++i) {
			if (offsets[i].y > maxY) {
				maxY = worldTrafo.pos.y + offsets[i].y;
			}
		}
		return maxY;
	}


	void Camera::WindowResized(const WindowResizedEvent& e)
	{
		width = e.newWidth;
		height = e.newHeight;
		needToRecalcOffset = true;
	}


	void Camera::Tick(float deltaTime) {}


	void Camera::Render() {}

}
