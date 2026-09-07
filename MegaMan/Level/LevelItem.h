#pragma once
#include <Util/Util.h>
#include <functional>
#include <string>
#include <vector>

struct MenuItem
{
	// 메뉴 선택 시 실행할 로직 저장을 위한 함수 포인터
	using OnSelected = std::function<void()>;

	// 생성자
	MenuItem(const std::string& text, OnSelected onSelected)
		: onSelected(onSelected)
	{
		Util::LoadTextFile(text, menuText);
	}

	// 메뉴 텍스트
	std::vector<std::string> menuText;

	// 메뉴를 선택했을 때 실행 로직
	OnSelected onSelected = nullptr;
};