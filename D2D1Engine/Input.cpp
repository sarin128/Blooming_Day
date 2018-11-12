#include"pch.h"
#include"App.h"
#include"Input.h"


Input::Input()
	:keyStateL_{0,},
	keyStateR_{0,}
{
	DirectInput8Create(GetModuleHandleA(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8A, (void**)&directInput_, nullptr);
	// Keyboard
	directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, nullptr);
	keyboard_->SetDataFormat(&c_dfDIKeyboard);
	keyboard_->SetCooperativeLevel(App::GetInstance()->GetWindow()->GetHwnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	keyboard_->Acquire();

	//Mouse
	directInput_->CreateDevice(GUID_SysMouse, &mouse_, nullptr);
	mouse_->SetDataFormat(&c_dfDIMouse);
	mouse_->SetCooperativeLevel(App::GetInstance()->GetWindow()->GetHwnd(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	mouse_->Acquire();
}
Input::~Input()
{
	if (mouse_)
	{
		mouse_->Unacquire();
		mouse_->Release();
	}
	if (keyboard_)
	{
		keyboard_->Unacquire();
		keyboard_->Release();
	}
	if (directInput_)
	{
		directInput_->Release();
	}
}

void Input::Update(void)
{
	HRESULT result = 0;
	memcpy_s(keyStateL_, sizeof(keyStateL_), keyStateR_, sizeof(keyStateR_));
	result = keyboard_->GetDeviceState(sizeof(keyStateR_), (LPVOID)&keyStateR_);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			keyboard_->Acquire();
		}
	}

	memcpy_s(rgbButtonsL_, sizeof(rgbButtonsL_),mouseState_.rgbButtons, sizeof(mouseState_.rgbButtons));
	result = mouse_->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState_);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mouse_->Acquire();
		}
	}
	
}

#define LKEY(key) (keyStateL_[key] & 0x80 ? true : false)
#define RKEY(key) (keyStateR_[key] & 0x80 ? true : false)

KeyState Input::GetKeyState(KeyCode key)
{
	if (LKEY(key) && RKEY(key)) return KeyState::KEYSTATE_STAY;
	if (!LKEY(key) && RKEY(key)) return KeyState::KEYSTATE_ENTER;
	if (LKEY(key) && !RKEY(key)) return KeyState::KEYSTATE_EXIT;
	return KeyState::KEYSTATE_NONE;
}

#undef LKEY
#undef RKEY

#define LKEY(key) (rgbButtonsL_[key] & 0x80 ? true : false)
#define RKEY(key) (mouseState_.rgbButtons[key] & 0x80 ? true : false)

KeyState Input::GetMouseState(MouseCode key)
{
	if (LKEY(key) && RKEY(key)) return KeyState::KEYSTATE_STAY;
	if (!LKEY(key) && RKEY(key)) return KeyState::KEYSTATE_ENTER;
	if (LKEY(key) && !RKEY(key)) return KeyState::KEYSTATE_EXIT;
	return KeyState::KEYSTATE_NONE;
}

#undef LKEY
#undef RKEY



Point2F Input::GetMousePos() const
{
	POINT mousePoint;
	GetCursorPos(&mousePoint);
	ScreenToClient(App::GetInstance()->GetWindow()->GetHwnd(), &mousePoint);
	return Point2F(mousePoint.x, mousePoint.y);
}


long Input::GetMouseDeltaX(void) const
{
	return mouseState_.lX;
}

long Input::GetMouseDeltaY(void) const
{
	return mouseState_.lY;
}

int Input::GetMouseWheel(void) const
{
	return (int)mouseState_.lZ;
}