#pragma once

#include <random>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>

namespace Util
{
	// 랜덤 엔진 반환 함수
	inline std::mt19937& GetRandomEngine()
	{
		// 프로그램 실행 중 1번만 생성되는 랜덤 엔진
		static std::mt19937 engine;
		return engine;
	}

	// 랜덤 시드 설정 함수
	inline void SetRandomSeed()
	{
		// 하드웨어 기반 나누 생성기(시드 값 제공)
		std::random_device randomDevice;

		// 랜덤 엔진에 종자값(시드) 설정
		GetRandomEngine().seed(randomDevice());
	}

	// 정수 함수
	inline int RandomRange(int min, int max)
	{
		// min에서 max까지 균등하게 부동소수점 난수를 생성해주는 분포 정의
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(GetRandomEngine());
	}

	// 부동 소수점 난수 함수
	inline float RandomRange(float min, float max)
	{
		// min에서 max까지 균등하게 부동소수점 난수를 생성해주는 분포 정의
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(GetRandomEngine());
	}

	inline void LoadTextFile(const std::string& path, std::vector<std::string>& vector)
	{
		std::ifstream file(path, std::ios::binary);
		if (!file.is_open())
		{
			assert(false && "failed to open test File\n");
			return;
		}

		std::string str;

		while (std::getline(file, str))
		{
			if (!str.empty() && str.back() == '\r')
			{
				str.pop_back();
			}

			vector.emplace_back(str);
		}
	}
}