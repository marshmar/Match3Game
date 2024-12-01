#pragma warning(disable:4996)
#include <iostream>
#include "Utils.h"
#include "InputManager.h"
#include "GameObject.h"


int main() {
	InputManager input;

	bool exit_flag = false;

	GameObject::Initialize();
	while (exit_flag == false) {

		GameObject::ClearFrame();
		input.readEveryFrame();
		GameObject::ProcessInput(exit_flag, input);
		GameObject::ThreeMatchCheck();
		GameObject::RecreateCard();
		GameObject::UpdateAll();
		GameObject::DrawAll();
		GameObject::RenderFrame();

	}
	
}