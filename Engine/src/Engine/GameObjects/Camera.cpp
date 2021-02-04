#include "pch.h"
#include "Camera.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	Camera::Camera(GameObject* parent, const std::string& name, const Transform& trafo, float width, float height) :
		GameObject(parent, name, trafo), width(width), height(height)
	{
		Subscribe<WindowResizedEvent>(SPH_BIND(Camera::WindowResized));
	}


	Point& Camera::Project(const Point& pos)
	{
		Point diff = (pos - trafo.pos).Rotate(-trafo.rot);
		return Point(2 * diff.x / width * trafo.scale.x, -2 * diff.y / height * trafo.scale.y);
	}


	void Camera::SetCurrent()
	{
		Renderer::SetCamera(this);
	}


	void Camera::WindowResized(const WindowResizedEvent& e)
	{
		width = e.newWidth;
		height = e.newHeight;
	}


	void Camera::TickThis(float deltaTime) {}

	void Camera::RenderThis() {}
}
