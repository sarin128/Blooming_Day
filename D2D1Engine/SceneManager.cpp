#include "pch.h"
#include "SceneManager.h"
#include"scene.h"
SceneManager::SceneManager()
{
	currentScene_ = new Scene();
}


SceneManager::~SceneManager()
{
	currentScene_->Dispose();
	delete currentScene_;
}

Scene * SceneManager::GetScene() const {
	if (Scene::isChanging_)
		return nextScene_;
	else
		return currentScene_;
}

void SceneManager::ChangeScene(Scene * scene)
{
	Scene::isChanging_ = false;
	Scene::tempEntities_.swap(scene->entities_);
	Scene::tempCollidables_.swap(scene->collidables_);
	nextScene_ = scene;
	currentScene_->Destroy();
	/*currentScene_->Dispose();
	delete currentScene_;*/
}

void SceneManager::Update()
{
	if (currentScene_->IsDestroyed())
	{
		currentScene_->Dispose();
		delete currentScene_;
		currentScene_ = nextScene_;
		nextScene_ = nullptr;
	}
	currentScene_->Update();

}

void SceneManager::Render()
{
	if (currentScene_)
	{
	
		
		currentScene_->Render();

		
	}
		
}
