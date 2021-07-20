#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class InputComponentCtrl : public wxPanel
	{
	public:
		InputComponentCtrl();
		InputComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32 width, const wxColour& color);
		virtual ~InputComponentCtrl();

		virtual inline uint32 GetWidth() { return GetSize().x; }
		virtual void SetWidth(uint32 width);
		virtual inline void Clear() { input->SetValue(""); }

		template <typename EventTag, typename Class, typename EventArg, typename EventHandler>
		void Bind(const EventTag& eventType, void (Class::* method)(EventArg&), EventHandler* handler) {
			input->Bind(eventType, method, handler);
		}

	protected:
		wxStaticText* titleText;
		wxTextCtrl* input;

		static const wxFont titleFont;
		static const uint32 border;
	};


	class IntComponentCtrl : public InputComponentCtrl
	{
	public:
		IntComponentCtrl();
		IntComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32 width, const wxColour& color,
			int32 min = INT32_MIN, int32 max = INT32_MAX);
		virtual ~IntComponentCtrl();

		inline void SetValue(int32 value) { this->value = value;  input->SetValue(wxString::Format(formater, value)); }
		inline int32 GetValue() {
			long realValue;
			input->GetValue().ToLong(&realValue);
			return realValue;
		}

	private:
		wxString formater;
		int32 value;
	};


	class UIntComponentCtrl : public InputComponentCtrl
	{
	public:
		UIntComponentCtrl();
		UIntComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32 width, const wxColour& color,
			uint32 min = 0, uint32 max = UINT32_MAX);
		virtual ~UIntComponentCtrl();

		inline void SetValue(uint32 value) { this->value = value;  input->SetValue(wxString::Format(formater, value)); }
		inline uint32 GetValue() {
			unsigned long realValue;
			input->GetValue().ToULong(&realValue);
			return realValue;
		}

	private:
		wxString formater;
		uint32 value;
	};


	class ByteComponentCtrl : public InputComponentCtrl
	{
	public:
		ByteComponentCtrl();
		ByteComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32 width, const wxColour& color, bool hex = false);
		virtual ~ByteComponentCtrl();

		inline void SetValue(byte value) { this->value = value;  input->SetValue(wxString::Format(formater, value)); }
		inline byte GetValue() {
			unsigned long realValue;
			input->GetValue().ToULong(&realValue);
			return realValue;
		}

	private:
		wxString formater;
		byte value;
	};


	class FloatComponentCtrl : public InputComponentCtrl
	{
	public:
		FloatComponentCtrl();
		FloatComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32 width, const wxColour& color, uint32 precision = 3);
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
		AngleComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32 width, const wxColour& color, uint32 precision = 3);
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