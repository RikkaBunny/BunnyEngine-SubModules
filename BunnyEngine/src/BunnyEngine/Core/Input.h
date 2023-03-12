#pragma once

#include "BunnyEngine/Core/Core.h"

namespace BE {

	class Input {

	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	};

}