#include "QuadTree.h"
#include <Actor/Actor.h>


namespace Pixel
{
	QuadTree::QuadTree(const Rect& bounds, int depth, int maxObjectsPerNode) : bounds(bounds), depth(depth), maxObjectsPerNode(maxObjectsPerNode)
	{
	}

	void QuadTree::Insert(Actor* actor)
	{
		if (!actor)
		{
			return;
		}

		if (!bounds.Intersects(actor->GetBounds()))
		{
			return;
		}

		// 아직 분리가 안됐으면
		if (!IsDivided())
		{
			if (depth >= maxDepth)
			{
				objects.emplace_back(actor);
				return;
			}

			Subdivide();

			// 기존에 있던 애들 재분배
			for (Actor* existing : objects)
			{
				topLeft->Insert(existing);
				topRight->Insert(existing);
				bottomLeft->Insert(existing);
				bottomRight->Insert(existing);
			}

			objects.clear();
		}

		// 자식에게 위임
		topLeft->Insert(actor);
		topRight->Insert(actor);
		bottomLeft->Insert(actor);
		bottomRight->Insert(actor);
	}

	void QuadTree::Query(const Rect& queryBounds, std::vector<Actor*>& result) const
	{
		if (!bounds.Intersects(queryBounds))
		{
			return;
		}

		for (Actor* actor : objects)
		{
			if (queryBounds.Intersects(actor->GetBounds()))
			{
				result.emplace_back(actor);
			}
		}

		if (IsDivided())
		{
			topLeft->Query(queryBounds, result);
			topRight->Query(queryBounds, result);
			bottomLeft->Query(queryBounds, result);
			bottomRight->Query(queryBounds, result);
		}
	}
	void QuadTree::Reset(const Rect& newBounds)
	{
		bounds = newBounds;
		objects.clear();

	}

	void QuadTree::Clear()
	{
	}
	bool QuadTree::Subdivide()
	{
		if (depth >= maxDepth)
		{
			return false;
		}

		float halfWidth = bounds.width / 2.0f;
		float halfHeight = bounds.height / 2.0f;

		topLeft = new QuadTree(Rect(bounds.x, bounds.y, halfWidth, halfHeight), depth + 1, maxObjectsPerNode);
		topRight = new QuadTree(Rect(bounds.x + halfWidth, bounds.y, halfWidth, halfHeight), depth + 1, maxObjectsPerNode);
		bottomLeft = new QuadTree(Rect(bounds.x, bounds.y + halfHeight, halfWidth, halfHeight), depth + 1, maxObjectsPerNode);
		bottomRight = new QuadTree(Rect(bounds.x + halfWidth, bounds.y + halfHeight, halfWidth, halfHeight), depth + 1, maxObjectsPerNode);

		return true;
	}
}