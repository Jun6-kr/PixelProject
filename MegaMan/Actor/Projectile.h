#pragma once
#include <Actor/Actor.h>
#include <Render/AnimationClip.h>


struct ProjectileDefination
{
	const Pixel::AnimationClip* animation = nullptr;

	int width = 0;
	int height = 0;

	float speed = 0.0f;
	int damage = 0;
	float lifeTime = 0.0f;

	uint32_t blockMask = 0;
	uint32_t damageMask = 0;
};

class GameLevel;

class Projectile : public Pixel::Actor
{
	TYPE_DECLARATIONS(Projectile, Actor)

public:
	Projectile();

	virtual void OnSpawn(GameLevel* level);
	void SetProjectileInfo(ProjectileDefination definition, const Pixel::Vector2& position, const Pixel::Vector2& direction = Pixel::Vector2(1, 0), GameLevel* level = nullptr);

	virtual Pixel::Rect GetBounds() const;

protected:
	virtual void Tick(float deltaTime) override;
	virtual void Draw(const Pixel::Vector2& screenOffset, const Pixel::Vector2& screenEnd) override;

protected:
	ProjectileDefination projectileDef;
	int animIndex = 0;

	float posX = 0.0f;
	float posY = 0.0f;

	float elapsedTime = 0.0f;
	Pixel::Vector2 direction;

	const Pixel::AnimationClip* animation = nullptr;
};

