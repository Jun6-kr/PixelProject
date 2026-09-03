#pragma once
#include <Level\Level.h>
#include <Math/Rect.h>
#include <Physics/Collision.h>

class GameLevel : public Pixel::Level
{
	TYPE_DECLARATIONS(GameLevel, Level)

public:
	GameLevel(const std::string& path);

	bool UpdatePlayerPos(const Pixel::Vector2& position);

	Pixel::CollisionResult CheckCollision(Pixel::Rect rect);

protected:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:


protected:
	// 데이터 로드 시 지정
	int levelFullWidth = 0;
	int levelFullHeight = 0;

	// Setting.txt에서 지정
	Pixel::Vector2 camStart;
	Pixel::Vector2 camEnd;
	int cameraBoundOffset = 30;

	// 실시간 갱신
	int screenWidthOffset = 0;
	int screenHeightOffset = 0;
};