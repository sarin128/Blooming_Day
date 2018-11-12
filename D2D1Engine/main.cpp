#include"pch.h"
#include"App.h"

#include "Scene1.h"
#include "Login.h"

#if defined(_DEBUG)
int main()
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	RG2App->Init();

	RG2SceneManager->ChangeScene(new Scene1());
	//RG2SceneManager->ChangeScene(new Login());

	RG2App->MainLoop();
	
	RG2App->Dispose();
	
	return 0;
}