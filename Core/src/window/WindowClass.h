#pragma once
#include "NeoWin.h"
#include <string> 

namespace Neodot::window
{
	class IWindowClass
	{
	public:
		virtual ~IWindowClass() = default;
		virtual ATOM GetAtom() const = 0;
		virtual HINSTANCE GetInstance() const = 0;

	protected:
		// IWindowClass 파생클래스에서 IWindow 인터페이스 메시지 핸들러 쓸 수 있도록 ;;stateless
		static LRESULT ForwardMessage(class IWindow* pWnd, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	};

	class WindowClass : public IWindowClass
	{
	public:
		WindowClass(const std::wstring& className = L"Neodot_Default_Window_Class");
		ATOM GetAtom() const override;
		HINSTANCE GetInstance() const override;
		~WindowClass() override;
	private:
		//============
		// functions 
		//============
		// 윈도우 생성 직후 1회 사용되는 초기 메시지 핸들러
		static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		// 이후 일반 메시지 처리 핸들러
		static LRESULT CALLBACK HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		//============
		// data 
		//============
		HINSTANCE m_hInstance; // 모듈 핸들
		ATOM m_atom; // 클래스 등록 결과값
	};
}