#pragma once
#include"Math.h"
#include"KeyCode.h"
//TODO: rawinput으로 수정
#define KEY_MAXCOUNT  256

class Input 
{
private:
	LPDIRECTINPUT8A directInput_;
	LPDIRECTINPUTDEVICE8A keyboard_;
	LPDIRECTINPUTDEVICE8A mouse_;

	BYTE keyStateL_[KEY_MAXCOUNT];
	BYTE keyStateR_[KEY_MAXCOUNT];
	DIMOUSESTATE mouseState_;
	BYTE rgbButtonsL_[4];
public:
	Input();
	~Input();

	void Update(void);
	KeyState GetKeyState(KeyCode key);

#define DIK_MOUSELBUTTON 0 // 좌클릭
#define DIK_MOUSERBUTTON 1 // 우클릭
#define DIK_MOUSEMBUTTON 2 // 휠클릭
#define DIK_MOUSEXBUTTON 3 // 확장버튼클릭
	KeyState GetMouseState(MouseCode key);

	Point2F GetMousePos() const;
	
	long GetMouseDeltaX(void) const;
	long GetMouseDeltaY(void) const;

	// 내릴시( < 0) 음수(-120, -240, -360 ...), 올릴시( > 0) 양수(120, 240, 360 ...)
	int GetMouseWheel(void) const;

};

