#pragma once

#include"GameObject.h"

enum class CardType {
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
};

class Card: public GameObject
{
private: 
	bool isSelected;
public:

	CardType type;

	Card(const char* shape, const Vector2& pos, const Dimension& dim, CardType type): GameObject(shape, pos, dim), isSelected(false),type(type) { }

	~Card() { }

	void IsSelected(bool isSelected) {
		this->isSelected = isSelected;
	};

	void update() override;

	CardType getCardType() const { return this->type; }
};

