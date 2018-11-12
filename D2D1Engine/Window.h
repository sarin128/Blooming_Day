#pragma once
#include"Math.h"
//RG engine Graphic class에서 Window부분만 분리, 수정
class Window
{
private:
	HWND hwnd_;
	MSG msg_;
	RECT savedRect_;
	char cStrBuffer_[C_STR_BUFFER_SIZE];
	bool isFullscreen_;
	bool isResizable_;
public:
	Window();
	~Window();
	void SetTitle(const std::string& title);
	void SetPos(const Point2L& pos);
	void SetSize(const Size2U& size);
	void SetFullscreen(bool flag);
	void SetResizable(bool flag);
	void SetMaximizable(bool flag);
	void ToggleFullscreen();
	HWND GetHwnd();
	std::string GetTitle();
	Point2L GetPos();
	Size2U GetSize();
	void SetClipCursor(bool set);
	void Minimize();
	void Maximize();
	void Focus();
	void Foreground();
	void Activate();
	RECT GetWin();
	WNDCLASSEXA wc = { 0, };
	//
	bool IsRunning();
	bool IsResizable();
	bool IsMaximizable();
	bool IsMinimized();
	bool IsMaximized();
	bool IsFocused();
	bool IsActive();
	bool IsFullscreen();
	bool MsgLoop();
	wchar_t g_strText[256 * 5 + 1];

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void CALLBACK KeyboardProc(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
};

