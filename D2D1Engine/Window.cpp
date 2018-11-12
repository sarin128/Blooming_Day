#include "pch.h"
#include "Window.h"
#include"App.h"
#include"resource.h"

Window::Window()
	:isFullscreen_(App::GetInstance()->GetSettings()->isFullscreen),
	isResizable_(App::GetInstance()->GetSettings()->isResizable),
	msg_{0,},
	savedRect_{0,0,(LONG)App::GetInstance()->GetSettings()->windowWidth, (LONG)App::GetInstance()->GetSettings()->windowHeight },
	cStrBuffer_{0,}
{
	
	wc = { 0, };
	HINSTANCE hInstance = GetModuleHandleA(nullptr);
	wc.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(IDI_ICON1));
	wc.hIconSm = LoadIconA(hInstance, MAKEINTRESOURCEA(IDI_ICON1));
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.hInstance = hInstance;
	wc.lpszClassName = App::GetInstance()->GetSettings()->title.c_str();
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = &Window::WndProc;

	// 윈도우 클래스를 운영체제에 등록
	RegisterClassExA(&wc);

	int wx, wy;
	RECT rcWindow = { 0,  };
	DWORD style, exstyle;
	
	if (isFullscreen_)
	{
		wx = wy = 0L;
		rcWindow.right = GetSystemMetrics(SM_CXSCREEN);
		rcWindow.bottom = GetSystemMetrics(SM_CYSCREEN);
		style = WS_SYSMENU | WS_POPUP;
		exstyle =NULL;
	}
	else
	{
		wx = (GetSystemMetrics(SM_CXSCREEN) - App::GetInstance()->GetSettings()->windowWidth) / 2;
		wy = (GetSystemMetrics(SM_CYSCREEN) - App::GetInstance()->GetSettings()->windowHeight) / 2;
		rcWindow.right = App::GetInstance()->GetSettings()->windowWidth;
		rcWindow.bottom = App::GetInstance()->GetSettings()->windowHeight;
		style = WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION | (isResizable_ ? WS_THICKFRAME : 0 );
		exstyle = NULL;
	}

	// 가로 세로 크기를 클라이언트 크기 기준으로 맞춤
	AdjustWindowRectEx(&rcWindow, style, false, exstyle);



	// 윈도우를 생성
	hwnd_ = CreateWindowExA(exstyle, App::GetInstance()->GetSettings()->title.c_str(),
		App::GetInstance()->GetSettings()->title.c_str(), style,
		wx, wy, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top,
		NULL, NULL, wc.hInstance, NULL);
	
	HWND hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT("test"),
		WS_CHILD | WS_VISIBLE, 100, 20, 140,
		20, hwnd_, NULL, NULL, NULL);

	//SavedRect가 초기값이 fullscreen일때도 설정되게 수정함
	AdjustWindowRectEx(&savedRect_, WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION, false, NULL);

	ShowWindow(hwnd_, SW_SHOWNORMAL);
}


Window::~Window()
{
}

void Window::SetTitle(const std::string& title)
{
	SetWindowTextA(hwnd_, title.c_str());
}

void Window::SetPos(const Point2L& pos)
{
	SetWindowPos(hwnd_, NULL, pos.x, pos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::SetSize(const Size2U& size)
{
	//client 기준으로 설정하도록 수정함
	RECT tmp = { 0, 0, (LONG)size.width, (LONG)size.height };
	AdjustWindowRectEx(&tmp, GetWindowLongA(hwnd_, GWL_STYLE), false, GetWindowLongA(hwnd_, GWL_EXSTYLE));
	SetWindowPos(hwnd_, NULL, 0,0, tmp.right - tmp.left, tmp.bottom - tmp.top, SWP_NOZORDER | SWP_NOSIZE);
}

RECT Window::GetWin()
{
	RECT rc;
		POINT lt, rb;
		GetClientRect(hwnd_, &rc);
		lt.x = rc.left;
		lt.y = rc.top;
		rb.x = rc.right;
		rb.y = rc.bottom;
		ClientToScreen(hwnd_, &lt);
		ClientToScreen(hwnd_, &rb);
		rc.left = lt.x;
		rc.top = lt.y;
		rc.right = rb.x;
		rc.bottom = rb.y;
		return rc;
}


void Window::SetClipCursor(bool set) {
		RECT rc;
		if (set) {
		POINT lt, rb;
		GetClientRect(hwnd_, &rc);
		lt.x = rc.left;
		lt.y = rc.top;
		rb.x = rc.right;
		rb.y = rc.bottom;
		ClientToScreen(hwnd_, &lt);
		ClientToScreen(hwnd_, &rb);
		rc.left = lt.x;
		rc.top = lt.y;
		rc.right = rb.x;
		rc.bottom = rb.y;
		ClipCursor(&rc);
	}
	else {
		rc.left = 0;
		rc.top = 0;
		rc.right = GetSystemMetrics(SM_CXSCREEN);
		rc.bottom = GetSystemMetrics(SM_CYSCREEN);
		ClipCursor(&rc);
	}
}

void Window::SetFullscreen(bool flag)
{
	//변경하려는 상태가 현재상태와 같으면 아무일도 하지않음
	if (flag == isFullscreen_) return;

	isFullscreen_ = flag;
	if (flag)
	{
		GetWindowRect(hwnd_, &savedRect_);
		SetWindowLongA(hwnd_, GWL_STYLE, WS_SYSMENU | WS_POPUP);
		SetWindowPos(hwnd_, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
	else
	{
		SetWindowLongA(hwnd_, GWL_STYLE, WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION| (isResizable_ ? WS_THICKFRAME : 0));
		SetWindowPos(hwnd_, HWND_NOTOPMOST, savedRect_.left, savedRect_.top, savedRect_.right - savedRect_.left, savedRect_.bottom - savedRect_.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
}

void Window::SetResizable(bool flag)
{
	isResizable_ = flag;
	SetWindowLongA(hwnd_, GWL_STYLE, flag ?
		GetWindowLongA(hwnd_, GWL_STYLE) | WS_THICKFRAME :
		GetWindowLongA(hwnd_, GWL_STYLE) & ~WS_THICKFRAME);
}

void Window::SetMaximizable(bool flag)
{
	SetWindowLongA(hwnd_, GWL_STYLE, flag ?
		GetWindowLongA(hwnd_, GWL_STYLE) | WS_MAXIMIZEBOX :
		GetWindowLongA(hwnd_, GWL_STYLE) & ~WS_MAXIMIZEBOX);
}

void Window::ToggleFullscreen()
{
	isFullscreen_ = !isFullscreen_;
	if (isFullscreen_)
	{
		GetWindowRect(hwnd_, &savedRect_);
		SetWindowLongA(hwnd_, GWL_STYLE, WS_SYSMENU | WS_POPUP);
		SetWindowPos(hwnd_, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
	else
	{
		SetWindowLongA(hwnd_, GWL_STYLE, WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION | (isResizable_ ? WS_THICKFRAME : 0));
		SetWindowPos(hwnd_, HWND_NOTOPMOST, savedRect_.left, savedRect_.top, savedRect_.right - savedRect_.left, savedRect_.bottom - savedRect_.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
}

HWND Window::GetHwnd()
{
	return hwnd_;
}

std::string Window::GetTitle()
{
	cStrBuffer_[GetWindowTextA(hwnd_, cStrBuffer_, C_STR_BUFFER_SIZE)] = '\0';
	return cStrBuffer_;
}

Point2L Window::GetPos()
{
	RECT rc;
	GetWindowRect(hwnd_, &rc);
	return Point2L(rc.left, rc.top);
}

Size2U Window::GetSize()
{
	RECT rc;
	GetClientRect(hwnd_, &rc);
	return Size2U((UINT32)rc.right, (UINT32)rc.bottom);
}

void Window::Minimize()
{
	ShowWindow(hwnd_, SW_SHOWMINIMIZED);
}

void Window::Maximize()
{
	ShowWindow(hwnd_, SW_SHOWMAXIMIZED);
}

void Window::Focus()
{
	SetFocus(hwnd_);
}

void Window::Foreground()
{
	SetForegroundWindow(hwnd_);
}

void Window::Activate()
{
	SetActiveWindow(hwnd_);
}

bool Window::IsFullscreen()
{
	return isFullscreen_;
}

bool Window::MsgLoop()
{
	if (PeekMessageA(&msg_, NULL, 0U, 0U, PM_REMOVE))
	{
		//TranslateMessage(&msg_);
		DispatchMessageA(&msg_);
		return false;
	}
	return true;
}
LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcA(hwnd,msg, wParam, lParam);
}

bool Window::IsRunning()
{
	return !(msg_.message == WM_QUIT);
}

bool Window::IsResizable()
{
	return GetWindowLongA(hwnd_, GWL_STYLE) & WS_THICKFRAME ? true : false;
}

bool Window::IsMaximizable()
{
	return GetWindowLongA(hwnd_, GWL_STYLE) & WS_MAXIMIZEBOX ? true : false;
}

bool Window::IsMinimized()
{
	return IsIconic(hwnd_);
}

bool Window::IsMaximized()
{
	return IsZoomed(hwnd_);
}

bool Window::IsFocused()
{
	return GetFocus() == hwnd_;
}

bool Window::IsActive()
{
	return GetActiveWindow() == hwnd_;
}

void CALLBACK Window::KeyboardProc(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext) {
	wchar_t strScanCode[20];
	if (bKeyDown) {
		swprintf(strScanCode, 20, L"%c", nChar);
		lstrcat(g_strText, strScanCode);
	}
}