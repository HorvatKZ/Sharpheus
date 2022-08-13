#pragma once

#include "ButtonBase.hpp"


namespace Sharpheus {

	class SPH_EXPORT ImageButton : public ButtonBase
	{
	public:
		ImageButton(GameObject* parent, const std::string& name);
		virtual ~ImageButton();
		virtual void CopyFrom(GameObject* other) override;

		inline const Image* GetImg() { return img; }
		inline const Image* GetClickedImg() { return clickedImg; }

		inline void SetImg(const Image* img) { this->img = img; UpdateSizer(); }
		inline void SetClickedImg(const Image* clickedImg) { this->clickedImg = clickedImg; UpdateSizer(); }
		inline void SetImgFromPath(const std::string& path, bool filtered);
		inline void SetClickedImgFromPath(const std::string& path, bool filtered);

		virtual void Render() override;

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(ImageButton)

	protected:
		const Image* img = nullptr;
		const Image* clickedImg = nullptr;

		virtual bool Save(FileSaver& fs) override;

		virtual void UpdateSizer() override;
	};

}