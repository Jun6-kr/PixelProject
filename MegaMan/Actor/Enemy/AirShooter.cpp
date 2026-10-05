#include "AirShooter.h"
#include <Level/GameLevel.h>
#include <System/DebugSystem.h>

using namespace Pixel;

AirShooter::AirShooter()
{
}

void AirShooter::SetProjectileIndex(int index)
{
    JxDebug::DebugSystem::Get().Print("set index", index);
    projectileIndex = index;
}

void AirShooter::SetDeployTarget(const Pixel::Vector2& target)
{
    deployTarget = target;
    pathIndex = 0;
    speedPathIndex = 0.0f;

    if (std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner()))
    {
        aStar.GetScreenWorldPosition(Vector2(level->GetScreenOffsetX(), level->GetScreenOffsetY())
            , level->GetScreenWidth()
            , level->GetScreenHeight());
        deployPath = aStar.FindPath(position, deployTarget, level->GetBlocked());
    }

    state = AirShooterState::Deploy;
}

void AirShooter::Fire()
{
    state = AirShooterState::Fire;
}

void AirShooter::Tick(float deltaTime)
{
    switch (state)
    {
    case AirShooterState::Deploy:
        MoveToDeployPosition(deltaTime);
        break;

    case AirShooterState::Standby:
        break;

    case AirShooterState::Fire:
        posX += direction.x * fireSpeed * deltaTime;
        position.x = static_cast<int>(posX);
        break;
    default:
        break;
    }
}

void AirShooter::Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd)
{
    if (!deployPath.empty())
    {
        for (int ix = 0; ix < deployPath.size(); ++ix)
        {
            JxDebug::DebugSystem::Get().AddBox(deployPath[ix].x - screenOffset.x, deployPath[ix].y - screenOffset.y, width, height, 0x0D);
        }
        
        JxDebug::DebugSystem::Get().AddBox(deployTarget.x - screenOffset.x, deployTarget.y - screenOffset.y, width, height, 0x05);
    }

    super::Draw(screenOffset, screenEnd);
}

void AirShooter::MoveToDeployPosition(float deltaTime)
{
    if (!isActive)
    {
        return;
    }

    if (deployPath.empty())
    {
        state = AirShooterState::Standby;
        return;
    }

    float speed = projectileDef.speed * deltaTime;
    speedPathIndex += speed;
    int tempIndex = static_cast<int>(speedPathIndex);

    if (tempIndex >= deployPath.size() - 1)
    {
        tempIndex = static_cast<int>(deployPath.size() - 1);
    }

    pathIndex = tempIndex;

    Vector2 newPosition = GetPosition();
    newPosition.x = deployPath[pathIndex].x;
    newPosition.y = deployPath[pathIndex].y;
    SetPosition(newPosition);
    
    if (pathIndex >= deployPath.size() - 1)
    {
        pathIndex = 0;
        //SetActive(false);

        if (deployCallback)
        {
            JxDebug::DebugSystem::Get().Print("callback", projectileIndex);
            deployCallback(projectileIndex);
        }

        state = AirShooterState::Standby;
        return;
    }
}
