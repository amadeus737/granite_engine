#include "GameWindow.h"
#include <assert.h>

namespace Granite
{
	BITMAPINFO _bufferInfo;
	void* _screenBuffer;

	GameWindow::GameWindow(HINSTANCE instance, wchar_t* args)
		:
		_instance(instance),
		_args(args)
	{
		// Register window class
		WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, _HandleMsgSetup, 0, 0, instance, nullptr, nullptr, nullptr, nullptr, _windowName, nullptr };

		// Setup icons and cursor
		wc.hIconSm = nullptr;
		wc.hIcon = nullptr;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

		// Register
		RegisterClassEx(&wc);

		// Create the window rect
		RECT rect;
		rect.left = 350;
		rect.right = ScreenWidth + rect.left;
		rect.top = 100;
		rect.bottom = ScreenHeight + rect.top;
		AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		// Create the window
		hWnd = CreateWindow(_windowName, L"Granite Engine - v0.0a : Game Window", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, _instance, this);

		// Throw exception if something went terribly wrong
		if (!hWnd) {
			MessageBox(0, L"CreateWindowEx failed", 0, 0);
		}

		// Get device context
		_deviceContext = GetDC(hWnd);

		// Allocate screen buffer
		int bytesPerPixel = 4;
		_screenBuffer = VirtualAlloc(0, ScreenWidth * ScreenHeight * bytesPerPixel, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		// Setup bitmap info struct for StretchDIBits
		_bufferInfo.bmiHeader.biSize = sizeof(_bufferInfo.bmiHeader);
		_bufferInfo.bmiHeader.biWidth = ScreenWidth;
		// Negative height makes top left as coord sys origin for DrawPixel function...otherwise its bottom left
		_bufferInfo.bmiHeader.biHeight = -ScreenHeight;
		_bufferInfo.bmiHeader.biPlanes = 1;
		_bufferInfo.bmiHeader.biBitCount = 32;
		_bufferInfo.bmiHeader.biCompression = BI_RGB;

		// Show and update
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);
	}

	GameWindow::~GameWindow()
	{
		// unregister window class
		UnregisterClass(_windowName, _instance);
	}

	bool GameWindow::IsActive() const
	{
		return GetActiveWindow() == hWnd;
	}

	bool GameWindow::IsMinimized() const
	{
		return IsIconic(hWnd) != 0;
	}

	void GameWindow::ShowMessageBox(const std::wstring& title, const std::wstring& message, UINT type) const
	{
		MessageBox(hWnd, message.c_str(), title.c_str(), type);
	}

	bool GameWindow::ProcessMessage()
	{
		if (!_isRunning) return false;

		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				return false;
			}

		//	StretchDIBits(_deviceContext, 0, 0, ScreenWidth, ScreenHeight, 0, 0, ScreenWidth, ScreenHeight, _screenBuffer, &_bufferInfo, DIB_RGB_COLORS, SRCCOPY);
		}

		return true;
	}

	void GameWindow::RefreshScreenBuffer()
	{
		StretchDIBits(_deviceContext, 0, 0, ScreenWidth, ScreenHeight, 0, 0, ScreenWidth, ScreenHeight, _screenBuffer, &_bufferInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	LRESULT WINAPI GameWindow::_HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
		if (msg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			GameWindow* const pWnd = reinterpret_cast<GameWindow*>(pCreate->lpCreateParams);
			// sanity check
			assert(pWnd != nullptr);
			// set WinAPI-managed user data to store ptr to window class
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			// set message proc to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&GameWindow::_HandleMsgThunk));
			// forward message to window class handler
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		// if we get a message before the WM_NCCREATE message, handle with default handler
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT WINAPI GameWindow::_HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// retrieve ptr to window class
		GameWindow* const pWnd = reinterpret_cast<GameWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT GameWindow::HandleMsg(HWND window, UINT msg, WPARAM wp, LPARAM lp)
	{
		LRESULT result;
		switch (msg)
		{
			case WM_CLOSE:
				_isRunning = false;
				OnKill();

				break;
		}

		return DefWindowProc(window, msg, wp, lp);
	}

	void GameWindow::ClearScreen(uint32_t color)
	{
		uint32_t* pixel = (uint32_t*)_screenBuffer;
		for (int index = 0; index < ScreenWidth * ScreenHeight; ++index)
		{
			*pixel++ = color;
		}
	}

	void GameWindow::DrawPixel(int x, int y, uint32_t color)
	{
		DrawPixelArea(x, y, 1, 1, color);
	}

	void GameWindow::DrawPixelArea(int x, int y, int width, int height, uint32_t color)
	{
		for (int i = x; i < x + width; i++)
		{
			for (int j = y; j < y + height; j++)
			{
				uint32_t* pixel = (uint32_t*)_screenBuffer;
				pixel += j * ScreenWidth + i;
				*pixel = color;
			}
		}
	}
}