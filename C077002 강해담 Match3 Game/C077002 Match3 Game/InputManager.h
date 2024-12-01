#pragma warning(disable:4996)
#pragma once

#include<conio.h>
#include<iostream>
#include "Utils.h"

using namespace std;

//FSM ����
enum class KeyState {
	Idle,
	Pressed,
	Stay,
	Released,
	PressedReleased
};

// Mouse���� FSM ������ �����ϱ� ���� typedef
typedef KeyState MouseState;

class InputManager
{
	// Ű���� �Է� ���¸� �����ϱ� ���� KeyState Ÿ���� �迭
	KeyState keys[256];

	// �����Ϸ��� �ܼ�â
	HANDLE hStdin;

	// �����Ϸ��� �ܼ� ���
	DWORD fdwSaveOldMode;

	// �о�� �Է� �̺�Ʈ�� ������ ����
	INPUT_RECORD irInBuf[128];
	
	// Ű �̸��� ������ �迭
	char keyName[1024];

	// ���콺 �Է� ���¸� �����ϱ� ���� MouseStae=te Ÿ���� �迭
	MouseState mouse[2];

	// ���콺 ��ǥ(X, Y)
	COORD mousePosition;

	VOID ErrorExit(LPCSTR IpszMessage) {
		Debug::Log("%s\n", IpszMessage);
		SetConsoleMode(hStdin, fdwSaveOldMode);
		ExitProcess(0);
	}

	VOID KeyEventProc(KEY_EVENT_RECORD ker) {
		Debug::Log("Key event: bkeyDown: %d, wRepeatCount: %d, keyCode: %d, wScanCode: %d, uChar: %d, KeyState: %d",
			ker.bKeyDown, ker.wRepeatCount, ker.wVirtualKeyCode, ker.wVirtualScanCode, ker.uChar, ker.dwControlKeyState);

		switch (keys[ker.wVirtualKeyCode & 0x00ff]) {
		case KeyState::Idle:
			if (ker.bKeyDown == TRUE)
				keys[ker.wVirtualKeyCode] = KeyState::Pressed;
			break;
		case KeyState::Pressed:
			if (ker.bKeyDown == FALSE)
				keys[ker.wVirtualKeyCode] = KeyState::PressedReleased;
			else
				keys[ker.wVirtualKeyCode] = KeyState::Stay;
			break;
		case KeyState::Stay:
			if (ker.bKeyDown == FALSE)
				keys[ker.wVirtualKeyCode] = KeyState::Released;
			break;
		case KeyState::Released:
		case KeyState::PressedReleased:
			keys[ker.wVirtualKeyCode] = KeyState::Idle;
			break;
		}
	}

	VOID MouseEventProc(MOUSE_EVENT_RECORD mer){
#ifndef MOUSE_HWHEELED
#define MOUSE_HEHEELED 0x0008
#endif
		// Mouse�̺�Ʈ�� ���� �α׸� ������ �迭
		char log[128];
		memset(log, ' ', 128);
		log[127] = '\0';

		switch (mer.dwEventFlags) {
		case 0:
			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				switch (mouse[0]) {
				case MouseState::Idle:
					mouse[0] = MouseState::Pressed;
					mousePosition = mer.dwMousePosition;
					break;
				case MouseState::Pressed:
					mouse[0] = MouseState::Stay;
					mousePosition = mer.dwMousePosition;
					break;
				}
			}
			else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
				switch (mouse[1]){
				case MouseState::Idle:
					mouse[1] = MouseState::Pressed;
					mousePosition = mer.dwMousePosition;
					break;
				case MouseState::Pressed:
					mouse[1] = MouseState::Released;
					mousePosition = mer.dwMousePosition;
					break;
				}
				mousePosition = mer.dwMousePosition;
			}
			else {
				for (int i = 0; i < 2; i++) {
					switch (mouse[i]) {
					case MouseState::Stay:
						mouse[i] = MouseState::Released;
						mousePosition = mer.dwMousePosition;
						break;
					}
				}
			}
			break;
		case DOUBLE_CLICK:
			sprintf(log, " => double click");
			break;
		case MOUSE_HWHEELED:
			sprintf(log, " =>horizontal mouse wheel");
			break;
		case MOUSE_MOVED:
			mousePosition = mer.dwMousePosition;
			sprintf(log, " => vertical mouse wheel");
			break;
		default:
			sprintf(log, " => unknown");
			break;
		}
	}

	const char* vk2name(WORD keyCode) {
		UINT scanCode = MapVirtualKeyA(keyCode, MAPVK_VK_TO_VSC);
		LONG IParamValue = (scanCode << 16);
		GetKeyNameTextA(IParamValue, keyName, 1024);
		return keyName;
	}

public:
	InputManager() {
		DWORD fdwMode;

		for (int i = 0; i < 256; i++)
			keys[i] = KeyState::Idle;

		mouse[0] = MouseState::Idle;
		mouse[1] = MouseState::Idle;
		mousePosition = { 0, 0 };

		// �Է� ���� �ʱ�ȭ
		memset(irInBuf, 0, sizeof(INPUT_RECORD) * 128);

		// �ܼ�â ����
		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		if (hStdin == INVALID_HANDLE_VALUE)
			ErrorExit("GetStdHandle");

		if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
			ErrorExit("GetConsoleMode");

		fdwMode = (ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT) & ~ENABLE_QUICK_EDIT_MODE;
		if (!SetConsoleMode(hStdin, fdwMode))
			ErrorExit("SetConsoleMode");
	}

	virtual ~InputManager() {
		SetConsoleMode(hStdin, fdwSaveOldMode);
	}

	void readEveryFrame() {
		DWORD cNumRead = 0;
		
		if (GetNumberOfConsoleInputEvents(hStdin, &cNumRead) == 0) {
			Debug::Log("Input Reading Failure %d", GetLastError());
			return;
		}

		for (int i = 0; i < 256; i++) {
			switch (keys[i]) {
			case KeyState::Released:
			case KeyState::PressedReleased:
				keys[i] = KeyState::Idle;
				break;
			case KeyState::Pressed:
				keys[i] = KeyState::Stay;
				break;
			}
		}

		for (int i = 0; i < 2; i++) {
			switch (mouse[i]) {
			case MouseState::Released:
				mouse[i] = MouseState::Idle;
				break;
			case MouseState::Pressed:
				mouse[i] = MouseState::Stay;
				break;
			}
		}

		if (cNumRead == 0) {
			return;
		}

		if (!ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead))
			ErrorExit("ReadConsoleInput");

		for (int i = 0; i < (int)cNumRead; i++) {
			switch (irInBuf[i].EventType) {
			case KEY_EVENT:
				KeyEventProc(irInBuf[i].Event.KeyEvent);
				break;
			case MOUSE_EVENT:
				MouseEventProc(irInBuf[i].Event.MouseEvent);
				break;
			}
		}
	}

	auto getKeyDown(WORD keyCode) {
		bool result = keys[keyCode & 0x00ff] == KeyState::Pressed || keys[keyCode & 0x00ff] == KeyState::PressedReleased;
		if (result) Debug::Log("key %s pressed [%x]", vk2name(keyCode), keys[keyCode & 0x00ff]);
		return result;
	}

	auto getKey(WORD keyCode) {
		bool result = keys[keyCode & 0x00ff] == KeyState::Pressed 
			|| keys[keyCode & 0x00ff] == KeyState::Stay
			|| keys[keyCode & 0x00ff] == KeyState::PressedReleased
			|| keys[keyCode & 0x00ff] == KeyState::Released;
		if (result) Debug::Log("key %s staying [%x]", vk2name(keyCode), keys[keyCode & 0x00ff]);
		return result;
	}

	auto getKeyUp(WORD keyCode) {
		bool result = keys[keyCode & 0x00ff] == KeyState::Released || keys[keyCode & 0x00ff] == KeyState::PressedReleased;
		if (result) Debug::Log("key %s released [%x]", vk2name(keyCode), keys[keyCode & 0x00ff]);
		return result;
	}

	auto getMouseButton(int button) const {
		bool result = mouse[button] == MouseState::Pressed
			|| mouse[button] == MouseState::Stay
			|| mouse[button] == MouseState::Released;
		if (result) Debug::Log("mouse %d state staying [%x] on [%d %d]", button, mouse[button], mousePosition.X, mousePosition.Y);
		return result;
	}

	auto getMouseButtonDown(int button) const {
		bool result = mouse[button] == MouseState::Pressed;
		if (result) Debug::Log("mouse %d pressed [%x] on [%d %d]", button, mouse[button], mousePosition.X, mousePosition.Y);
		return result;
	}

	auto getMouseButtonUp(int button) const {
		bool result = mouse[button] == MouseState::Released;
		if (result) Debug::Log("mouse %d released [%x] on [%d %d]", button, mouse[button], mousePosition.X, mousePosition.Y);
		return result;
	}

	Position getMousePosition() const {
		return { mousePosition.X, mousePosition.Y };
	}
};

