#include<cstdlib>
#include "InputManager.h"
#include "GameObject.h"
#include "Canvas.h"
#include "Card.h"


Canvas GameObject::canvas(45, 49);
int GameObject::max_objs = 63;
GameObject** GameObject::objs = new GameObject * [GameObject::max_objs];

void GameObject::draw() { canvas.draw(shape, position, dimension); }

void GameObject::Initialize()
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < max_objs; i++)
		objs[i] = nullptr;

	GameObject* createdCard = nullptr;
	//auto 키워드: 선언된 변수의 초기화 식을 사용하여 해당 형식을 추론하도록 컴파일러에 지시, 초깃값의 형식에 맞춰 선언하는 인스턴스(변수)의 형식이 '자동'으로 결정
	auto dim = canvas.getDimension();
	float posX = 0;
	float posY = 0;
	for (int i = 0; i < max_objs; i++)
	{
		// 카드 배치
		createdCard = createCard(posX, posY);
		GameObject::Add(createdCard);
		posX += 5;
		if (posX >= canvas.getDimension().x) {
			posY += 7;
			posX = 0;
		}
	}
}

void GameObject::ProcessInput(bool& exit_flag, InputManager& input)
{
	Card* card = nullptr;
	static Card* selectedCard = nullptr;
	static int selectedCardIndex = 0;

	if (input.getKeyDown(0x51)) { // 'q'
		exit_flag = true;
	}
	if (input.getMouseButtonDown(0)) {
		auto mousePos = input.getMousePosition();
		for (int i = 0; i < max_objs; i++) {
			if (objs[i] == nullptr) continue;
			card = dynamic_cast<Card*>(objs[i]);
			if (card == nullptr) continue;
			
			// 마우스 클릭시 카드의 좌표 위에 있으면 그 카드를 선택.
			if ((mousePos.x > card->getPosition().x && mousePos.x <= card->getPosition().x + card->getDimension().x)
				&& (mousePos.y > card->getPosition().y && mousePos.y <= card->getPosition().y + card->getDimension().y)) {
				selectedCard = card;
				if (selectedCard == nullptr) continue;
				selectedCard->IsSelected(true);
				selectedCardIndex = i;
				break;
			}
		}
	}
	if (input.getMouseButton(0)) {
		auto mousePos = input.getMousePosition();
	}
	if (input.getMouseButtonUp(0)) {
		auto mousePos = input.getMousePosition();
		static SwapDirection swapDir = SwapDirection::NONE;
		if (selectedCard == nullptr) return;

		// Drag & Drop Right
		if ((mousePos.x > selectedCard->getPosition().x + selectedCard->getDimension().x)
			&& (mousePos.y > selectedCard->getPosition().y && mousePos.y <= selectedCard->getPosition().y + selectedCard->getDimension().y)) {
			if (mousePos.x > canvas.getDimension().x) return;
			swapDir = SwapDirection::RIGHT;
		}
		// Drag & Drop left
		else if ((mousePos.x < selectedCard->getPosition().x)
			&& (mousePos.y > selectedCard->getPosition().y && mousePos.y <= selectedCard->getPosition().y + selectedCard->getDimension().y)) {
			swapDir = SwapDirection::LEFT;
		}
		// Drag & Drop Up
		else if ((mousePos.y < selectedCard->getPosition().y)
			&& (mousePos.x > selectedCard->getPosition().x && mousePos.x <= selectedCard->getPosition().x + selectedCard->getDimension().x)) {
			swapDir = SwapDirection::UP;
		}
		// Drag & Drop down
		else if ((mousePos.y > selectedCard->getPosition().y + selectedCard->getDimension().y)
			&& (mousePos.x > selectedCard->getPosition().x && mousePos.x <= selectedCard->getPosition().x + selectedCard->getDimension().x)) {
			if (mousePos.y >= canvas.getDimension().y) return;
			swapDir = SwapDirection::DOWN;
		}
		else
			swapDir = SwapDirection::NONE;

		Swap(selectedCard, swapDir, selectedCardIndex);
		selectedCard->IsSelected(false);
		selectedCard = nullptr;
	}
}

void GameObject::Add(GameObject* obj)
{
	if (obj == nullptr) return;

	for (int i = 0; i < max_objs; i++)
	{
		if (objs[i] != nullptr) continue;
		objs[i] = obj;
		break;
	}
}

GameObject* GameObject::createCard(float posX, float posY) {
	GameObject* createdCard = nullptr;
	switch (rand() % 5)
	{
	case 0:
		createdCard = new Card("       #    #    #    #    #       ", Vector2{ posX, posY }, Dimension{ 5, 7 }, CardType::ONE);
		break;
	case 1:
		createdCard = new Card("      ###    #  ###  #    ###      ", Vector2{ posX, posY }, Dimension{ 5, 7 }, CardType::TWO);
		break;
	case 2:
		createdCard = new Card("      ###    #  ###    #  ###      ", Vector2{ posX, posY }, Dimension{ 5, 7 }, CardType::THREE);
		break;
	case 3:
		createdCard = new Card("      # #  # #  ###    #    #      ", Vector2{ posX, posY }, Dimension{ 5, 7 }, CardType::FOUR);
		break;
	case 4:
		createdCard = new Card("      ###  #    ###    #  ###      ", Vector2{ posX, posY }, Dimension{ 5, 7 }, CardType::FIVE);
		break;
	}

	return createdCard;
}

void GameObject::Swap(Card* selectedCard, SwapDirection& swapDir, int selectedCardIndex) {
	if (selectedCard == nullptr) return;
	Card* tmp = nullptr;
	Vector2 tmpPosition = selectedCard->getPosition();
	switch (swapDir) {
	case SwapDirection::NONE:
		break;
	case SwapDirection::RIGHT:
		if (selectedCardIndex + 1 > max_objs) return;
		if (objs[selectedCardIndex + 1] == nullptr) return;

		selectedCard->setPosition(objs[selectedCardIndex + 1]->getPosition());
		objs[selectedCardIndex + 1]->setPosition(tmpPosition);

		std::swap(objs[selectedCardIndex], objs[selectedCardIndex + 1]);
		swapDir = SwapDirection::NONE;
		break;
	case SwapDirection::LEFT:
		if (selectedCardIndex - 1 < 0) return;
		if (objs[selectedCardIndex - 1] == nullptr) return;

		selectedCard->setPosition(objs[selectedCardIndex - 1]->getPosition());
		objs[selectedCardIndex - 1]->setPosition(tmpPosition);

		std::swap(objs[selectedCardIndex], objs[selectedCardIndex - 1]);
		swapDir = SwapDirection::NONE;
		break;
	case SwapDirection::UP:
		if (selectedCardIndex - 9 < 0) return;
		if(objs[selectedCardIndex - 9] == nullptr) return;

		selectedCard->setPosition(objs[selectedCardIndex - 9]->getPosition());
		objs[selectedCardIndex - 9]->setPosition(tmpPosition);

		std::swap(objs[selectedCardIndex], objs[selectedCardIndex - 9]);
		swapDir = SwapDirection::NONE;
		break;
	case SwapDirection::DOWN:
		if (selectedCardIndex + 9 > max_objs) return;
		if (objs[selectedCardIndex + 9] == nullptr) return;

		selectedCard->setPosition(objs[selectedCardIndex + 9]->getPosition());
		objs[selectedCardIndex + 9]->setPosition(tmpPosition);

		std::swap(objs[selectedCardIndex], objs[selectedCardIndex + 9]);
		swapDir = SwapDirection::NONE;
		break;
	}
}

void GameObject::ThreeMatchCheck() {
	int w = 1;
	// 가로체크
	while(true) {
		if (dynamic_cast<Card*>(objs[w - 1])->getCardType() == dynamic_cast<Card*>(objs[w])->getCardType()
			&& dynamic_cast<Card*>(objs[w])->getCardType() == dynamic_cast<Card*>(objs[w + 1])->getCardType()) {
			objs[w - 1]->setAlive(false);
			objs[w]->setAlive(false);
			objs[w + 1]->setAlive(false);
		}
		if (++w % 9 == 8) {
			w += 2;
		}
			
		if (w + 2 > max_objs) {
			break;
		}
			
	}
	w = 9;
	// 세로체크
	while (true) {
		if (dynamic_cast<Card*>(objs[w-9])->getCardType() == dynamic_cast<Card*>(objs[w])->getCardType()
			&& dynamic_cast<Card*>(objs[w])->getCardType() == dynamic_cast<Card*>(objs[w + 9])->getCardType()) {
			objs[w - 9]->setAlive(false);
			objs[w]->setAlive(false);
			objs[w + 9]->setAlive(false);
		}
		w += 9;
		if (w + 9 >= max_objs) {
			w = (w % 9) + 10;
		}
		if (w + 9 == max_objs - 1) {
			break;
		}
	}
}
void GameObject::RecreateCard() {
	for (int i = 0; i < max_objs; i++) {
		if (objs[i] == nullptr) continue;
		if (objs[i]->isDied()) {
			Vector2 objPos = objs[i]->getPosition();
			delete objs[i];
			objs[i] = nullptr;
			objs[i] = createCard(objPos.x, objPos.y);
		}
	}
};

void GameObject::UpdateAll() {
	for (int i = 0; i < max_objs; i++)
	{
		if (objs[i] == nullptr) continue;
		objs[i]->update();
	}
}
void GameObject::ClearFrame(){
	canvas.clear();
}

void GameObject::DrawAll()
{
	for (int i = 0; i < max_objs; i++)
	{
		if (objs[i] == nullptr) continue;
		objs[i]->draw();
	}
}

void GameObject::RenderFrame()
{
	canvas.render();
	Sleep(10);
}