#pragma once

#include "Control.hpp"


namespace Sharpheus {

	class SPH_EXPORT CheckBase : public Control
	{
	public:
		CheckBase(GameObject* parent, const std::string& name, Shape* shape);
		virtual ~CheckBase();
		virtual void CopyFrom(GameObject* other) override;

		inline Image* GetCheckedImg() { return checkedImg; }
		inline Image* GetUncheckedImg() { return uncheckedImg; }
		inline bool IsTextOnRight() { return textOnRight; }

		inline void SetCheckedImg(Image* checkedImg) { this->checkedImg = checkedImg; UpdateSizer(); }
		inline void SetUncheckedImg(Image* uncheckedImg) { this->uncheckedImg = uncheckedImg; }
		inline void SetTextOnRight(bool textOnRight) { this->textOnRight = textOnRight; }

		void SetCheckedImgFromPath(const std::string& path, bool filtered);
		void SetUncheckedImgFromPath(const std::string& path, bool filtered);

		virtual inline bool GetState() override { return isChecked; }
		inline bool IsChecked() { return isChecked; }

		virtual bool Load(FileLoader& fl) override;

	protected:
		Image* checkedImg = nullptr;
		Image* uncheckedImg = nullptr;
		bool isChecked = false;
		bool textOnRight = true;

		virtual bool Save(FileSaver& fs) override;

		virtual void Render() override;

		virtual inline bool DoesChangeOnRelease() override { return false; }
		virtual void ChangeOnClick() = 0;
		virtual void UpdateSizer() override;
	};

}