#pragma warning(disable:4996)
#pragma once

#include <iostream>
#include <Windows.h>
#include <string.h>
#include <math.h>

struct Vector2 {
	float x;
	float y;

	Vector2(float x, float y) : x(x), y(y) {}

	// 벡터 크기 제곱 반환
	float sqrMagnitude() const { return x * x + y * y; }
	// 벡터 크기 반환
	float magnitude() const { return sqrt(sqrMagnitude()); }
	// 다른 벡터와의 거리가 0.5f 미만이면 true 반환
	bool equalApproximately(const Vector2& other) const { return distance(other) < 0.5f; }
	// 다른 벡터와의 거리 계산
	float distance(const Vector2& other) const { return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y)); }
	// 단위 벡터 계산
	Vector2 unit() const {
		if (equalApproximately({ 0.0f, 0.0f })) return { 1.0f, 0.0f };
		float mag = magnitude();
		return { this->x / mag, this->y / mag };
	}

	friend float sqrDistance(const Vector2 one, const Vector2 another) { return { one.distance(another) }; }
};

struct Position {
	int x;
	int y;

	Position(int x, int y): x(x), y(y) { }
};

typedef Position Dimension;

class Borland {
public:
	// 콘솔 커서 설정
	static void Initialize() {
		// 콘솔 크기 설정
		std::system("mode con:cols=170 lines=50");
		std::system("chcp 437");

		CONSOLE_CURSOR_INFO cci;
		cci.dwSize = 25;
		cci.bVisible = FALSE;	

		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	}

	// 커서의 X좌표 위치 반환
	static int WhereX() {
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.X;
	}

	// 커서의 Y좌표 위치 반환
	static int WhereY() {
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.Y;
	}

	// 커서를 매개변수 값의 X,Y 좌표로 이동
	static void GotoXY(int x, int y) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _COORD{ (SHORT)x, (SHORT)y });
	}

	// GotoXY 오버로딩
	static void GotoXY(const Position* pos) {
		if (!pos) return;
		GotoXY((*pos).x, (*pos).y);
	}

	// GotoXY 오버로딩
	static void GotoXY(const Position& pos) {
		GotoXY(pos.x, pos.y);
	}
};

class Debug {
	static int nDebugLine;
	static char whiteSpaces[160];

public:
	static void Log(const char* fmt, ...);
};

