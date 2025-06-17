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
		// IWindowClass �Ļ�Ŭ�������� IWindow �������̽� �޽��� �ڵ鷯 �� �� �ֵ��� ;;stateless
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
		// ������ ���� ���� 1ȸ ���Ǵ� �ʱ� �޽��� �ڵ鷯
		static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		// ���� �Ϲ� �޽��� ó�� �ڵ鷯
		static LRESULT CALLBACK HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		//============
		// data 
		//============
		HINSTANCE m_hInstance; // ��� �ڵ�
		ATOM m_atom; // Ŭ���� ��� �����
	};
}