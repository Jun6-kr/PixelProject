#include "ScreenBuffer.h"
#include <string>
#include <cassert>

namespace Pixel
{
	ScreenBuffer::ScreenBuffer(const Vector2& screenSize) : size(screenSize)
	{
		// 콘솔 버퍼 생성
		buffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE
			, FILE_SHARE_READ | FILE_SHARE_WRITE
			, nullptr
			, CONSOLE_TEXTMODE_BUFFER
			, nullptr);

		// 값 확인
		assert(buffer != INVALID_HANDLE_VALUE);


		CONSOLE_FONT_INFOEX fontInfo{};
		fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);

		fontInfo.dwFontSize.X = 8;   // 문자 가로 크기
		fontInfo.dwFontSize.Y = 8;   // 문자 세로 크기

		wcscpy_s(fontInfo.FaceName, L"Consolas");

		BOOL result = SetCurrentConsoleFontEx(buffer, FALSE, &fontInfo);
		//assert(result == TRUE);
		
		// 화면 버퍼 크기 설정
		result = SetConsoleScreenBufferSize(buffer, size);
		assert(result == TRUE);

		COORD YAP = ::GetLargestConsoleWindowSize(buffer);

		result = SetConsoleActiveScreenBuffer(buffer);
		assert(result == TRUE);

		// 화면 창 크기 설정
		SMALL_RECT rect = {};
		//rect.Top = 0;
		//rect.Left = 0;
		rect.Right = static_cast<short>(size.x - 1);
		rect.Bottom = static_cast<short>(size.y - 1);
		result = SetConsoleWindowInfo(buffer, TRUE, &rect);

		// 결과 확인
		assert(result == TRUE);

		
		// 직접 만든 콘솔의 커서 끄기
		CONSOLE_CURSOR_INFO info;
		result = GetConsoleCursorInfo(buffer, &info);
		assert(result == TRUE);
		
		// 커서 안보이게 설정
		info.bVisible = FALSE;
		result = SetConsoleCursorInfo(buffer, &info);
		assert(result == TRUE);


		CONSOLE_SCREEN_BUFFER_INFOEX cbi = { sizeof(cbi) };
		GetConsoleScreenBufferInfoEx(buffer, &cbi);

		// ColorTable은 COLORREF(0x00BBGGRR) 16개 배열 — 인덱스 0~15
		cbi.ColorTable[0] = RGB(30, 28, 26);		// Outline
		cbi.ColorTable[1] = RGB(255, 255, 255);		// White
		cbi.ColorTable[2] = RGB(200, 175, 140);		// Mega man Face
		cbi.ColorTable[3] = RGB(58, 110, 180);		// Mega man Body
		cbi.ColorTable[4] = RGB(100, 160, 215);		// Mega man Body Highlight

		cbi.ColorTable[5] = RGB(18, 26, 45);		// DarkBlue
		cbi.ColorTable[6] = RGB(225, 205, 125);		// Yellow
		cbi.ColorTable[7] = RGB(91, 68, 52);		// Brown
		cbi.ColorTable[8] = RGB(111, 153, 164);		// LightCyan
		cbi.ColorTable[9] = RGB(58, 66, 75);		// DarkGary
		cbi.ColorTable[10] = RGB(64, 114, 76);		// LightGreen
		cbi.ColorTable[11] = RGB(110, 112, 116);	// Gray
		cbi.ColorTable[12] = RGB(210, 65, 90);		// Wine



		cbi.ColorTable[13] = RGB(225, 95, 35);		// 
		cbi.ColorTable[14] = RGB(0, 255, 0);		// 

		int windowWidth = cbi.srWindow.Right - cbi.srWindow.Left + 1;
		int windowHeight = cbi.srWindow.Bottom - cbi.srWindow.Top + 1;
		OutputDebugStringA(("[" + std::to_string(windowWidth) + ", " + std::to_string(windowHeight) + "]" + "\n").c_str());


		// bug: SetConsoleScreenBufferInfoEx는 srWindow.Right/Bottom을 -1 해서 넘겨야 함
		cbi.srWindow.Right -= 1;
		cbi.srWindow.Bottom -= 1;

		result = SetConsoleScreenBufferInfoEx(buffer, &cbi);
		if (!result)
		{
			DWORD err = GetLastError();
			OutputDebugStringA(("SetConsoleScreenBufferInfoEx failed: " + std::to_string(err) + "\n").c_str());
		}
	}

	ScreenBuffer::~ScreenBuffer()
	{
		// 콘솔 닫기
		if (buffer)
		{
			CloseHandle(buffer);
		}
	}

	void ScreenBuffer::Clear() const
	{
		// 콘솔 전체를 지우는 함수
		// 공백 문자를 화면 전체에 한 번에 설정

		// 화면에 설정된 글자 수
		DWORD writtenCount = 0;

		BOOL result = FillConsoleOutputCharacterA(buffer
			, ' '
			, size.x * size.y
			, Vector2::Zero
			, &writtenCount);

		assert(result == TRUE);
	}

	void ScreenBuffer::Draw(const CHAR_INFO* const charInfo) const
	{
		// charInfo는 2차원 배열 (1차원 배열에 2차원 배열 정보를 기록)

		// 설정한 글자 영역 - L T R B
		SMALL_RECT rect = { 0, 0, static_cast<short>(size.x - 1), static_cast<short>(size.y - 1) };

		// 콘솔에 CHAR_INFO 타입으로 글자 쓰는 함수
		BOOL result = WriteConsoleOutputA(buffer
			, charInfo
			, size
			, Vector2::Zero
			, &rect);

		assert(result == TRUE);
	}
}