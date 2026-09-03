#include "GameLevel.h"
#include <Util/Util.h>
#include <Render/Renderer.h>
#include <Input/Input.h>
#include <System/DebugSystem.h>
#include <Debug/DebugAdapter.h>

using namespace Pixel;
using namespace JxDebug;

GameLevel::GameLevel(const std::string& path) : camStart(Vector2::Zero), camEnd(Vector2(285, 165))
{
	Util::LoadTextFile(path, levelData);

	levelFullWidth = static_cast<int>(levelData[0].length());
	levelFullHeight = static_cast<int>(levelData.size());
}

bool GameLevel::UpdatePlayerPos(const Pixel::Vector2& position)
{
	bool isMoved = true;

	int pivotX = camEnd.x / 2;
	int pivotY = camEnd.y / 2;

	if (position.x >= pivotX)
	{
		if (camEnd.x + screenWidthOffset < levelFullWidth)
		{
			screenWidthOffset += 2;
			isMoved = false;
		}
		else
		{
			screenWidthOffset = levelFullWidth - camEnd.x;
		}
	}

	if (position.y >= pivotY)
	{
		if (camEnd.y + screenHeightOffset < levelFullHeight)
		{
			screenHeightOffset += position.y - pivotY;
			isMoved = false;
		}
		else
		{
			screenHeightOffset = levelFullHeight - camEnd.y;
		}
	}

	return isMoved;
}

CollisionResult GameLevel::CheckCollision(Rect rect)
{
	CollisionResult result = {};

	// AABB로 검사 진행 이전 검사 대상 
	for (std::shared_ptr<Actor>& actor : actorList)
	{
		Rect bound = actor->GetBounds();
		
		JxDebug::DebugBox box = DebugAdapter::Get().MakeDebugBox(
			Vector2(static_cast<int>(rect.left), static_cast<int>(rect.top))
			, static_cast<int>(rect.width)
			, static_cast<int>(rect.height));
		JxDebug::DebugSystem::Get().AddBox(box);
		
		box = DebugAdapter::Get().MakeDebugBox(
			Vector2(static_cast<int>(bound.left), static_cast<int>(bound.top))
			, static_cast<int>(bound.width)
			, static_cast<int>(bound.height));
		JxDebug::DebugSystem::Get().AddBox(box);

		if (rect.Compare(bound))
		{
			switch (actor->GetActorType())
			{
			case ActorType::Enemy:
			case ActorType::Terrain:
				result.hasCollision = true;
				result.isBlocked = true;
				result.actor = actor;
				break;
			case ActorType::Bullet:
				result.hasCollision = true;
				//result.direction
				result.actor = actor;
				break;
			default:
				break;
			}

		}
	}

	return result;
}

void GameLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	
	if (Input::Get().GetKeyDown('P'))
	{
		DebugSystem::Get().SetDebugMode(!DebugSystem::Get().IsDebugMode());
	}
}

void GameLevel::Draw()
{
	for (int index = 0; index < levelData.size(); ++index)
	{
		int x = 0;

		for (const char ch : levelData[index])
		{
			if (camStart.x + screenWidthOffset <= x && x <= camEnd.x + screenWidthOffset
				&& camStart.y + screenHeightOffset <= index && index <= camEnd.y + screenHeightOffset)
			{
				Renderer::Get().Submit(" ", Vector2(x - screenWidthOffset, index - screenHeightOffset), (ch != '*') ? ch : 0xFF, 0);
			}

			++x;
		}
	}

	for (std::shared_ptr<Actor>& actor : actorList)
	{
		// 검증 - 활성화 되지 않았으면 건너뛰기
		if (!actor->IsActive())
		{
			continue;
		}

		Vector2 start = camStart - cameraBoundOffset;
		Vector2 end = camEnd + cameraBoundOffset;
		
		Vector2 pos = actor->GetPosition();

		if (pos.x >= start.x && pos.x < end.x && pos.y >= start.y && pos.y < end.y)
		{
			actor->Draw(camStart, camEnd);
		}
	}
}
