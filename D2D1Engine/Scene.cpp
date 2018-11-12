#include "pch.h"
#include "Scene.h"
#include"Entity.h"
#include"Camera.h"
#include"Math.h"
#include"App.h"
#include"Collider.h"
#include"RigidBody.h"
#include"AABBCollider.h"
#include"CircleCollider.h"
bool Scene::isChanging_ = false;
std::vector<Entity*> Scene::tempEntities_(0);
std::vector<Entity*> Scene::tempCollidables_(0);

void Scene::RegisterEntity(Entity * e)
{
	if (Scene::isChanging_)
	{
		tempEntities_.push_back(e);
	}
	else
	{
		entities_.push_back(e);
	}
		
}

void Scene::UnregisterEntity(Entity * e)
{
	if (Scene::isChanging_)
	{
		auto i = std::find(tempEntities_.begin(), tempEntities_.end(), e);
		tempEntities_.erase(i);
	}
	else
	{
		auto i = std::find(entities_.begin(), entities_.end(), e);
		entities_.erase(i);
	}
}

void Scene::RegisterCollidable(Entity * e)
{
	if (Scene::isChanging_)
	{
		tempCollidables_.push_back(e);
	}
	else
	{
		collidables_.push_back(e);
	}
}

void Scene::UnregisterCollidable(Entity * e)
{
	if (Scene::isChanging_)
	{
		auto i = std::find(tempCollidables_.begin(), tempCollidables_.end(), e);
		tempCollidables_.erase(i);
	}
	else
	{
		auto i = std::find(collidables_.begin(), collidables_.end(), e);
		collidables_.erase(i);
	}
}

void Scene::Dispose()
{
	for (auto& i : children_)
		delete i;
	children_.clear();
	tempEntities_.clear();
	tempCollidables_.clear();
}

Scene::Scene()
	:Entity(false),
	camera(this)
{
	Scene::isChanging_ = true;
}


Scene::~Scene()
{
}


const std::vector<Entity*>& Scene::GetEntities(void) const
{
	return entities_;
}

void Scene::Render()
{
	matrix =
		matrix*
		D2D1::Matrix3x2F::Scale(scale.x, scale.y, scaleCenter);

	App::GetInstance()->GetGraphics()->GetDeviceContext()->SetTransform(matrix);

	for (auto i : components_)
	{
		if ((i.second)->IsEnabled())
		{
			(i.second)->OnRender();
		}
	}

	if (isAutoRenderEnabled)
	{
		for (auto i : children_)
		{
			if (i->IsEnabled())
				i->Render();
		}
	}

	OnRender();
}

void Scene::Update()
{

	matrix = D2D1::Matrix3x2F::Translation(pos.x - center.x, pos.y - center.y);
	for (auto i : components_)
	{
		if ((i.second)->IsEnabled())
		{
			(i.second)->OnUpdate();
		}
	}

	if (isAutoUpdateEnabled)
	{
		for (auto i : children_)
		{
			if (i->IsEnabled())
			{
				i->Update();
			}

		}
	}


	OnUpdate();



	children_.erase(std::remove_if(children_.begin(), children_.end(), [](Entity* element) {
		const LifeState tmp = element->GetLifeState();
		if (tmp == Destroyed)
		{
			delete element;
			return true;
		}
		else if (tmp == Detached)
		{
			element->parent_ = nullptr;
			element->lifeState_ = Alive;
			return true;
		}
		return false;
	}), children_.end());
}

void Scene::OnUpdate()
{
	Entity::OnUpdate();
	for (auto iter = collidables_.begin(); iter != collidables_.end(); ++iter)
	{
		for (auto iter2 = next(iter); iter2 != collidables_.end(); ++iter2)
		{
			Collider* a = (*iter)->GetComponent<Collider>();
			Collider* b = (*iter2)->GetComponent<Collider>();
			if (!a || !b || !a->IsEnabled() || !b->IsEnabled()) continue;
			if (a->IsCollision(b))
			{
				a->onCollision_((*iter2));
				b->onCollision_((*iter));
				
			}
			else
				continue;

			RigidBody* ra = (*iter)->GetComponent<RigidBody>();
			RigidBody* rb = (*iter2)->GetComponent<RigidBody>();
			if (!ra || !rb || (ra->isFixed && rb->isFixed)) continue;

			//TODO:물리코드 분리

			//사각형-사각형
			if(a->GetID() == typeid(AABBCollider) &&
				b->GetID() == typeid(AABBCollider))
			{
				const Rect& r1 = ((AABBCollider*)a)->GetRectWithWorldPos();
				const Rect& r2 = ((AABBCollider*)b)->GetRectWithWorldPos();
				//교집합을 구한다.
				const Rect& intersection =r1.GetIntersection(r2);
				//X 방향 충돌일 때
				if (intersection.GetHeight() > intersection.GetWidth())
				{
					//r1이 왼쪽에 있을 때 
					if (r1.GetCenterX() < r2.GetCenterX())
					{
						//둘다 fixed가 아닐때
						if (!ra->isFixed && !rb->isFixed)
						{
							//반반씩 빼준다.
							//왼쪽에있는건 -,오른쪽에있는건 +
							(*iter)->pos.x -= intersection.GetWidth() / 2;
							(*iter2)->pos.x += intersection.GetWidth() / 2;
						}
						//ra가 fixed일때
						else if (ra->isFixed)
						{
							//rb(iter2)만 빼준다.
							(*iter2)->pos.x += intersection.GetWidth();
						}
						else if (rb->isFixed)
						{
							(*iter)->pos.x -= intersection.GetWidth();
						}
						ra->isTouchingRight = true;
						rb->isTouchingLeft = true;
					}
					//r1이 오른쪽에 있을 때
					else
					{
						if (!ra->isFixed && !rb->isFixed)
						{
							(*iter)->pos.x += intersection.GetWidth() / 2;
							(*iter2)->pos.x -= intersection.GetWidth() / 2;
						}
						else if (ra->isFixed)
						{
							(*iter2)->pos.x -= intersection.GetWidth();
						}
						else if (rb->isFixed)
						{
							(*iter)->pos.x += intersection.GetWidth();
						}
						rb->isTouchingRight = true;
						ra->isTouchingLeft = true;
					}
				}
				//y방향 충돌일 때
				else
				{
					//r1이 위쪽에 있을 때
					if (r1.GetCenterY() < r2.GetCenterY())
					{
						if (!ra->isFixed && !rb->isFixed)
						{
							(*iter)->pos.y -= intersection.GetHeight() / 2;
							(*iter2)->pos.y += intersection.GetHeight() / 2;
						}
						else if (ra->isFixed)
						{
							(*iter2)->pos.y += intersection.GetHeight();
						}
						else if (rb->isFixed)
						{
							(*iter)->pos.y -= intersection.GetHeight();
								
						}
						if (rb->gravity < 0.f)
						{
							rb->gravity = -rb->gravity;
							rb->isTouchingTop = true;
						}
						if (ra->gravity > 0.f)
						{
							ra->gravity = rb->gravity;
							ra->isTouchingBottom = true;
						}
					}
					//r1이 아래쪽에 있을 때
					else
					{
						if (!ra->isFixed && !rb->isFixed)
						{
							(*iter)->pos.y += intersection.GetHeight() / 2;
							(*iter2)->pos.y -= intersection.GetHeight() / 2;
						}
						else if (ra->isFixed)
						{
							(*iter2)->pos.y -= intersection.GetHeight();
						}
						else if (rb->isFixed)
						{
							(*iter)->pos.y += intersection.GetHeight();
						}
						if (ra->gravity < 0.f)
						{
							ra->gravity = -ra->gravity;
							ra->isTouchingTop = true;
						}
						if (rb->gravity > 0.f)
						{
							rb->gravity = ra->gravity;
							rb->isTouchingBottom = true;
						}
					}
				}
			}
			//원-원
			else if (a->GetID() == typeid(CircleCollider) &&
				b->GetID() == typeid(CircleCollider))
			{
				const Circle& c1 = ((CircleCollider*)a)->GetCircleWithWorldPos();
				const Circle& c2 = ((CircleCollider*)b)->GetCircleWithWorldPos();
				float intersection = c1.radius + c2.radius - Distance(c1.center, c2.center);
				float angleC1C2 = Angle(c1.center, c2.center);
				float angleC2C1 = NormalizeDegree(angleC1C2 +180);

				if (!ra->isFixed && !rb->isFixed)
				{
					//반반씩
					(*iter)->pos -= Vec2F(intersection * Cos(angleC1C2),
									intersection * Sin(angleC1C2)) / 2;
					(*iter2)->pos -= Vec2F(intersection * Cos(angleC2C1),
									intersection * Sin(angleC2C1)) / 2;
				}
				else if (ra->isFixed)
				{
					(*iter2)->pos -= Vec2F(intersection * Cos(angleC2C1),
									intersection * Sin(angleC2C1));
					if (angleC2C1 > 0)
					{
						rb->gravity *= (Sin(angleC2C1-180)+1);
					}
					else if (angleC2C1 < 0 && rb->gravity < 0)
					{
						rb->gravity = -rb->gravity*(Sin(angleC2C1+180));
					}
				}
				else if (rb->isFixed)
				{
					(*iter)->pos -= Vec2F(intersection * Cos(angleC1C2),
						intersection * Sin(angleC1C2));
					if (angleC1C2 > 0)
					{
						ra->gravity *= (Sin(angleC1C2-180)+1);
					}
					else if (angleC1C2 < 0 && ra->gravity < 0)
					{
						ra->gravity = -ra->gravity*(Sin(angleC1C2+180));
					}
				}
			}
			//원 - 사각형
			else if((a->GetID() == typeid(CircleCollider) &&
				b->GetID() == typeid(AABBCollider)) ||
				(a->GetID() == typeid(AABBCollider) &&
				b->GetID() == typeid(CircleCollider)))
			{
				Circle circle;
				Rect rect;
				int circleNum = 0;
				//iter == 1 , iter2 == 2
				//int circleNum = 0;
				//int rectNum = 0;
				if (a->GetID() == typeid(CircleCollider))
				{
					circle = ((CircleCollider*)a)->GetCircleWithWorldPos();
					rect = ((AABBCollider*)b)->GetRectWithWorldPos();
					circleNum = 1;
				}
				else
				{
					circle = ((CircleCollider*)b)->GetCircleWithWorldPos();
					rect = ((AABBCollider*)a)->GetRectWithWorldPos();
					circleNum = 2;
				}
				Vec2F nearest(
					Clamp(circle.center.x, rect.left, rect.right),
					Clamp(circle.center.y, rect.top, rect.bottom));

				float angle12 = circleNum == 1 ? Angle(circle.center, nearest) : Angle(nearest, circle.center);
				float angle21 = NormalizeDegree(angle12 + 180);

				float intersection = circle.radius - Distance(nearest, circle.center);


				if (!ra->isFixed && !rb->isFixed)
				{
					(*iter)->pos -= Vec2F(intersection * Cos(angle12),
									intersection * Sin(angle12)) / 2;
					(*iter2)->pos -= Vec2F(intersection * Cos(angle21),
									intersection * Sin(angle21)) / 2;
				}
				else if (ra->isFixed)
				{

					(*iter2)->pos -= Vec2F(intersection * Cos(angle21),
									intersection * Sin(angle21));
					if ((int)angle21 == 90 && rb->gravity > 0.f)
					{
						rb->gravity = 0.f;
						rb->isTouchingBottom = true;
					}
					else if ((int)angle21 ==-90 && rb->gravity < 0.f)
					{
						rb->gravity = -rb->gravity;
					}
						

				}
				else if (rb->isFixed)
				{

					(*iter)->pos -= Vec2F(intersection * Cos(angle12),
						intersection * Sin(angle12));
					if ((int)angle12 == 90 && ra->gravity > 0.f)
					{
						ra->gravity = 0.f;
						ra->isTouchingBottom = true;
					}
					else if ((int)angle12 == -90 && ra->gravity < 0.f)
					{
						ra->gravity = -ra->gravity;
					}
						
				}
			}

		}
	}

	camera.Update();
}

Entity * Scene::FindEntity(Tag tag)
{
	auto iter = std::find_if(entities_.begin(), entities_.end(), [tag](Entity* entity)
	{
		return entity->tag == tag;
	});
	
	if (iter == entities_.end())
		return nullptr;
	else
		return *iter;
}

Entity * Scene::FindEntityIf(std::function<bool(Entity*)> pred)
{
	auto iter = std::find_if(entities_.begin(), entities_.end(), pred);

	if (iter == entities_.end())
		return nullptr;
	else
		return *iter;
}


