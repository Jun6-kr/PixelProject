#include "AStarBullet.h"

using namespace Pixel;

AStarBullet::AStarBullet()
{}

std::vector<Vector2> AStarBullet::FindPath(const Vector2& startPosition, const Vector2& goalPosition, std::vector<std::vector<int>>& grid)
{
    return std::vector<Vector2>();
}

void AStarBullet::DisplayGridWithPath(std::vector<std::vector<int>>& grid, const std::vector<Vector2>& path)
{}

void AStarBullet::Clear()
{}

Node* AStarBullet::CreateNode(const Vector2 & position, Node * parent)
{
    return nullptr;
}

std::vector<Vector2> AStarBullet::ConstructPath(Node* destination)
{
    return std::vector<Vector2>();
}

float AStarBullet::CalculateHeuristic(const Vector2& current, const Vector2& goal) const
{
    return 0.0f;
}

bool AStarBullet::IsValidGrid(const std::vector<std::vector<int>>& grid) const
{
    return false;
}

bool AStarBullet::IsInRange(int x, int y, const std::vector<std::vector<int>>& grid) const
{
    return false;
}

bool AStarBullet::IsDiagonalBlocked(const Vector2& current, const Direction& direction, const std::vector<std::vector<int>>& grid) const
{
    return false;
}

Node* AStarBullet::FindOpenNode(int x, int y) const
{
    return nullptr;
}

bool AStarBullet::IsInClosedList(int x, int y) const
{
    return false;
}

bool AStarBullet::IsDestination(const Node* node) const
{
    return false;
}

void AStarBullet::ClearVisualization(std::vector<std::vector<int>>& grid) const
{}

void AStarBullet::DisplayGrid(std::vector<std::vector<int>>&grid) const
{}
