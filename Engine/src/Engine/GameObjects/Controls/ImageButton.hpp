#pragma once

#include "ButtonBase.hpp"


namespace Sharpheus {

	class SPH_EXPORT ImageButton : public ButtonBase
	{
	public:
		ImageButton(GameObject* parent, const std::string& name);
		virtual ~ImageButton();
		virtual void CopyFrom(GameObject* other) override;

		inline Image* GetImg() { return img; }
		inline Image* GetClickedImg() { return clickedImg; }

		inline void SetImg(Image* img) { this->img = img; UpdateSizer(); }
		inline void SetClickedImg(Image* clickedImg) { this->clickedImg = clickedImg; UpdateSizer(); }
		inline void SetImgFromPath(const std::string& path, bool filtered);
		inline void SetClickedImgFromPath(const std::string& path, bool filtered);

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(ImageButton)

	protected:
		Image* img = nullptr;
		Image* clickedImg = nullptr;

		virtual bool Save(FileSaver& fs) override;

		virtual void Render() override;

		virtual void UpdateSizer() override;
	};

}