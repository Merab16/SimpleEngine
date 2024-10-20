#pragma once
#include "Keys.h"

namespace SimpleEngine {

	class Input {
	private:
		static bool keysPressed_[static_cast<size_t>(KeyCode::KEY_LAST)];

	private:



	public:
		static bool IsKeyPressed(KeyCode key_code);
		static void PressKey(KeyCode key_code);
		static void ReleaseKey(KeyCode key_code);



	};




}

