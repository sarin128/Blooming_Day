#include "pch.h"
#include "App.h"


App::App()
{
}


App::~App()
{
}

void App::Init()
{
	std::locale::global(std::locale(""));

	settings_ = new Settings();
	window_ = new Window();
	graphics_ = new Graphics();
	input_ = new Input();
	soundManager_ = new SoundManager();
	textureManager_ = new TextureManager();
	sceneManager_ = new SceneManager();
	timer_ = new Timer();
}

void App::MainLoop()
{


	while (GetWindow()->IsRunning())
	{
		timer_->Tick();
		if (GetWindow()->MsgLoop())
		{

		}
	
		

		soundManager_->Update();
		textureManager_->Update();
		sceneManager_->Update();
		input_->Update();

		graphics_->Render();
		
	}
}

void App::Dispose()
{
	delete sceneManager_;
	delete textureManager_;
	delete soundManager_;
	delete input_;
	delete graphics_;
	delete window_;
	delete settings_;
	delete timer_;
}
