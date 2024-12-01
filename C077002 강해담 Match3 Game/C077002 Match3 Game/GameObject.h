#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream>
#include "Utils.h"

class Card;
class Canvas;
class InputManager;


enum class SwapDirection {
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class GameObject
{
private:
	
	char* shape;
	bool isAlive;
	Dimension dimension;
	Vector2 position;
	Vector2 direction;

protected:
	
	static Canvas canvas;
	static int max_objs;
	static GameObject** objs;

public:
	GameObject(const char* shape, const Vector2& pos = { 0.0f, 0.0f }, const Dimension& dim = { 1,1 }, const Vector2& dir = { 0.0f, 0.0f })
		: shape(new char[strlen(shape) + 1]), position(pos), dimension(dim), direction(dir), isAlive(true)
	{
	if (this->shape != nullptr)
		strcpy(this->shape, shape);
	}

	Vector2 getPosition() const { return position; }
	void	setPosition(const Vector2& pos) { position = pos; }

	Vector2 getDirection() const { return direction; }
	void	setDirection(const Vector2& dir) { direction = dir.unit(); }

	Dimension getDimension() const { return dimension; }
	void	changeDimension(const Dimension& dim) { dimension = dim; }

	const char* getShape() const { return this->shape; } // getter
	void	setShape(const char* shape) { strcpy(this->shape, shape); } // setter
	int		getShapeSize() const { return (int)strlen(this->shape); }

	void setAlive(bool isAlive) {
		this->isAlive = isAlive;
	}
	bool isDied() const { return this->isAlive == false; }

	virtual ~GameObject() {
		delete[] this->shape;
		this->shape = nullptr;
	}

	virtual void draw();

	virtual void update() {}
	
	static void Initialize();
	static void ProcessInput(bool& exit_flag, InputManager& input);
	static void Add(GameObject* obj);
	static GameObject* createCard(float posX, float posY);
	static void Swap(Card* card, SwapDirection& swapDir, int selectedCardIndex);
	static void ThreeMatchCheck();
	static void RecreateCard();
	static void ClearFrame();
	static void UpdateAll();
	static void DrawAll();
	static void RenderFrame();

};

