#pragma once
#include"Singleton.h"
#include"Window.h"
#include"Settings.h"
#include"Graphics.h"
#include"Input.h"
#include"SoundManager.h"
#include"TextureManager.h"
#include"SceneManager.h"
#include"Timer.h"
#define RG2App App::GetInstance()
#define RG2Window App::GetInstance()->GetWindow()
#define RG2Graphics App::GetInstance()->GetGraphics()
#define RG2Settings App::GetInstance()->GetSettings()
#define RG2Input App::GetInstance()->GetInput()
#define RG2SoundManager App::GetInstance()->GetSoundManager()
#define RG2TextureManager App::GetInstance()->GetTextureManager()
#define RG2SceneManager App::GetInstance()->GetSceneManager()
#define RG2Timer App::GetInstance()->GetTimer()
class App: public Singleton<App>
{
private:
	Settings* settings_;
	Window* window_;
	Graphics* graphics_;
	Input* input_;
	SoundManager* soundManager_;
	TextureManager* textureManager_;
	SceneManager* sceneManager_;
	Timer* timer_;
public:
	App();
	~App();
	
	Settings* GetSettings() const {	return settings_; }
	Window* GetWindow() const { return window_; }
	Graphics* GetGraphics() const { return graphics_; }
	Input* GetInput() const { return input_; }
	SoundManager* GetSoundManager() const { return soundManager_; }
	TextureManager* GetTextureManager() const { return textureManager_; }
	SceneManager* GetSceneManager() const { return sceneManager_; }
	Timer* GetTimer() const { return timer_; }

	void Init();

	void MainLoop();

	void Dispose();
};

