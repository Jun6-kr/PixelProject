#pragma once
#include <Render/Sprite.h>
#include <Render/AnimationClip.h>
#include <string>
#include <unordered_map>

class SpriteManager
{
public:
	SpriteManager();
	~SpriteManager() = default;

	const Pixel::AnimationClip* GetClip(const std::string& key);

private:
	void LoadDefaultSprite();

private:
	std::unordered_map<std::string, Pixel::AnimationClip> playerSpirtes;
};

