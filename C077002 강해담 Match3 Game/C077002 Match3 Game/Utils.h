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

	// ���� ũ�� ���� ��ȯ
	float sqrMagnitude() const { return x * x + y * y; }
	// ���� ũ�� ��ȯ
	float magnitude() const { return sqrt(sqrMagnitude()); }
	// �ٸ� ���Ϳ��� �Ÿ��� 0.5f �̸��̸� true ��ȯ
	bool equalApproximately(const Vector2& other) const { return distance(other) < 0.5f; }
	// �ٸ� ���Ϳ��� �Ÿ� ���
	float distance(const Vector2& other) const { return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y)); }
	// ���� ���� ���
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
	// �ܼ� Ŀ�� ����
	static void Initialize() {
		// �ܼ� ũ�� ����
		std::system("mode con:cols=170 lines=50");
		std::system("chcp 437");

		CONSOLE_CURSOR_INFO cci;
		cci.dwSize = 25;
		cci.bVisible = FALSE;	

		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	}

	// Ŀ���� X��ǥ ��ġ ��ȯ
	static int WhereX() {
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.X;
	}

	// Ŀ���� Y��ǥ ��ġ ��ȯ
	static int WhereY() {
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.Y;
	}

	// Ŀ���� �Ű����� ���� X,Y ��ǥ�� �̵�
	static void GotoXY(int x, int y) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _COORD{ (SHORT)x, (SHORT)y });
	}

	// GotoXY �����ε�
	static void GotoXY(const Position* pos) {
		if (!pos) return;
		GotoXY((*pos).x, (*pos).y);
	}

	// GotoXY �����ε�
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

