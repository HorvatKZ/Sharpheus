#pragma once

#include "Engine/GameObjects/Camera.hpp"


namespace Sharpheus {

    class ViewPortCamera : public Camera
    {
    public:
        ViewPortCamera() : Camera(nullptr, "VP_Camera") {}
        virtual ~ViewPortCamera() = default;

        inline float GetWidth() override { return customWidth; }
        inline float GetHeight() override { return customHeight; }
        inline float GetOGWidth() override { return customWidth; }
        inline float GetOGHeight() override { return customHeight; }

        inline void SetCustomRect(float width, float height) {
            customWidth = width;
            customHeight = height;
            SetSizer(customWidth, customHeight);
        }
             
    private:
        float customHeight = 0.f;
        float customWidth = 0.f;
    };

}