#include "AirShooter.h"
#include <Level/GameLevel.h>
#include <System/DebugSystem.h>

using namespace Pixel;

AirShooter::AirShooter()
{
}

void AirShooter::SetDeployTarget(const Pixel::Vector2& target)
{
    deployTarget = target;
    pathIndex = 0;

    if (std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner()))
    {
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
    super::Tick(deltaTime);

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
    JxDebug::DebugSystem::Get().AddBox(deployTarget.x - screenOffset.x, deployTarget.y - screenOffset.y, width, height, 0x0D);
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

    moveTimer += projectileDef.speed * deltaTime;

    while (moveTimer >= 1.0f && pathIndex < deployPath.size() - 1)
    {
        moveTimer -= 1.0f;
        ++pathIndex;

        posX = static_cast<float>(deployPath[pathIndex].x);
        posY = static_cast<float>(deployPath[pathIndex].y);

        std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner());

        CollisionResult result = level->CheckBlockCollision(this, GetBounds());
   

        Vector2 newPosition = GetPosition();
        newPosition.x = deployPath[pathIndex].x;
        newPosition.y = deployPath[pathIndex].y;
        SetPosition(newPosition);
    }

    if (pathIndex >= deployPath.size() - 1)
    {
        //pathIndex = 0;
        //SetActive(false);

        state = AirShooterState::Standby;
        return;
    }
}
