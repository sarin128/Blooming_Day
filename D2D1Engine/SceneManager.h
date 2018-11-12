#pragma once
class Scene;
class Entity;
class SceneManager
{
private:
	Scene* currentScene_;
	Scene* nextScene_;
public:
	SceneManager();
	~SceneManager();

	Scene* GetScene() const;
	void ChangeScene(Scene* scene);



	void Update();
	void Render();
};

