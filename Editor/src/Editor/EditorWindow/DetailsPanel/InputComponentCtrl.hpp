#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class InputComponentCtrl : public wxPanel
	{
	public:
		InputComponentCtrl();
		InputComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32_t width, const wxColour& color);
		virtual ~InputComponentCtrl();

		virtual inline uint32_t GetWidth() { return GetSize().x; }
		virtual void SetWidth(uint32_t width);
		virtual inline void Clear() { input->SetValue(""); }

		template <typename EventTag, typename Class, typename EventArg, typename EventHandler>
		void Bind(const EventTag& eventType, void (Class::* method)(EventArg&), EventHandler* handler) {
			input->Bind(eventType, method, handler);
		}

	protected:
		wxStaticText* titleText;
		wxTextCtrl* input;

		static wxFont titleFont;
	};


	class IntComponentCtrl : public InputComponentCtrl
	{
	public:
		IntComponentCtrl();
		IntComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32_t width, const wxColour& color,
			int32_t min = INT32_MIN, int32_t max = INT32_MAX);
		virtual ~IntComponentCtrl();

		inline void SetValue(int32_t value) { this->value = value;  input->SetValue(wxString::Format(formater, value)); }
		inline int32_t GetValue() {
			long realValue;
			input->GetValue().ToLong(&realValue);
			return realValue;
		}

	private:
		wxString formater;
		int32_t value;
	};


	class UIntComponentCtrl : public InputComponentCtrl
	{
	public:
		UIntComponentCtrl();
		UIntComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32_t width, const wxColour& color,
			uint32_t min = 0, uint32_t max = UINT32_MAX);
		virtual ~UIntComponentCtrl();

		inline void SetValue(uint32_t value) { this->value = value;  input->SetValue(wxString::Format(formater, value)); }
		inline uint32_t GetValue() {
			unsigned long realValue;
			input->GetValue().ToULong(&realValue);
			return realValue;
		}

	private:
		wxString formater;
		uint32_t value;
	};


	class ByteComponentCtrl : public InputComponentCtrl
	{
	public:
		ByteComponentCtrl();
		ByteComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32_t width, const wxColour& color, bool hex = false);
		virtual ~ByteComponentCtrl();

		inline void SetValue(uint8_t value) { this->value = value;  input->SetValue(wxString::Format(formater, value)); }
		inline uint8_t GetValue() {
			unsigned long realValue;
			input->GetValue().ToULong(&realValue);
			return realValue;
		}

	private:
		wxString formater;
		uint8_t value;
	};


	class FloatComponentCtrl : public InputComponentCtrl
	{
	public:
		FloatComponentCtrl();
		FloatComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32_t width, const wxColour& color, uint32_t precision = 3);
		virtual ~FloatComponentCtrl();

		inline void SetValue(float value) { input->SetValue(wxString::Format(formater, value)); }
		inline float GetValue() {
			double realValue;
			input->GetValue().ToDouble(&realValue);
			return realValue;
		}

	private:
		wxString formater;
		float value;
	};


	class AngleComponentCtrl : public InputComponentCtrl
	{
	public:
		AngleComponentCtrl();
		AngleComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32_t width, const wxColour& color, uint32_t precision = 3);
		virtual ~AngleComponentCtrl();

		inline void SetValue(float value) {
			NormalizeAngle(value);
			this->value = value; 
			input->SetValue(wxString::Format(formater, value));
		}
		inline float GetValue() {
			double realValue;
			input->GetValue().ToDouble(&realValue);
			return realValue;
		}

	private:
		wxString formater;
		float value;

		inline void NormalizeAngle(float& angle) {
			while (angle > 180.f) {
				angle -= 360.f;
			}

			while (angle < -180.f) {
				angle += 360.f;
			}
		}
	};

}