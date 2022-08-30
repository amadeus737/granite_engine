#pragma once

#include "Core.h"
#include <Windows.h>
#include <string>
#include <functional>

namespace Granite
{
	// for granting special access to hWnd only for Graphics constructor
	class GRANITE_API HWNDKey
	{
	public:
		HWNDKey(const HWNDKey&) = delete;
		HWNDKey& operator=(HWNDKey&) = delete;

	protected:
		HWNDKey() = default;

	protected:
		HWND hWnd = nullptr;
	};

	class GRANITE_API  GameWindow : public HWNDKey
	{
	public:
		GameWindow(HINSTANCE instance, wchar_t* args);
		GameWindow(const GameWindow&) = delete;
		GameWindow& operator=(const GameWindow&) = delete;
		~GameWindow();

		bool IsActive() const;
		bool IsMinimized() const;
		void ShowMessageBox(const std::wstring& title, const std::wstring& message, UINT type = MB_OK) const;

		std::function<void()> OnKill;

		static void ClearScreen(uint32_t color);
		static void DrawPixel(int x, int y, uint32_t color);
		static void DrawPixelArea(int x, int y, int width, int height, uint32_t color);
		void RefreshScreenBuffer();

		void Kill()
		{
			PostQuitMessage(0);
		}

		// returns false if quitting
		bool ProcessMessage();

		const std::wstring& GetArgs() const
		{
			return _args;
		}

	private:
		static LRESULT WINAPI _HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI _HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		static constexpr wchar_t _windowName[] = L"Granite Engine";
		HINSTANCE _instance = nullptr;
		std::wstring _args;
		bool _isRunning = true;
		HDC _deviceContext;
		static constexpr int ScreenWidth = 800;
		static constexpr int ScreenHeight = 600;
	};
}
