#pragma once
#include"Math.h"
#include"KeyCode.h"
//TODO: rawinput���� ����
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

#define DIK_MOUSELBUTTON 0 // ��Ŭ��
#define DIK_MOUSERBUTTON 1 // ��Ŭ��
#define DIK_MOUSEMBUTTON 2 // ��Ŭ��
#define DIK_MOUSEXBUTTON 3 // Ȯ���ưŬ��
	KeyState GetMouseState(MouseCode key);

	Point2F GetMousePos() const;
	
	long GetMouseDeltaX(void) const;
	long GetMouseDeltaY(void) const;

	// ������( < 0) ����(-120, -240, -360 ...), �ø���( > 0) ���(120, 240, 360 ...)
	int GetMouseWheel(void) const;

};

