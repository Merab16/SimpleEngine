#include "SimpleEngineCore/Input.h"



namespace SimpleEngine {

	bool Input::keysPressed_[static_cast<size_t>(KeyCode::KEY_LAST)] = {};


	bool Input::IsKeyPressed(KeyCode key_code) {
		return keysPressed_[static_cast<size_t>(key_code)];
	}

	void Input::PressKey(KeyCode key_code) {
		keysPressed_[static_cast<size_t>(key_code)] = true;
	}

	void Input::ReleaseKey(KeyCode key_code) {
		keysPressed_[static_cast<size_t>(key_code)] = false;
	}


}