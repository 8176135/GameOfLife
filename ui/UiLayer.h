//
// Created by username on 22/09/2021.
//

#ifndef GAME_OF_LIFE_UILAYER_H
#define GAME_OF_LIFE_UILAYER_H

#include "../main.fwd.h"
#include <vector>
#include <functional>
#include "../shader.h"
#include "unordered_set"

enum class ButtonState {
	Hover,
	Active, // (De)Pressed,
	None,
};

struct ButtonInfo {
	glm::mat4 elementTransform;
	std::function<void(CallbackContext &)> callback;
	ButtonState currentState;
	std::unordered_set<int> mouseButtonDown;
//	TextureData ,
};

class UiLayer {
	Shader uiShader;
	std::vector<ButtonInfo> buttons;

public:
	UiLayer();

	void RenderLayer();

	/// Returns true if hit a ui element, so stop propagating.
	bool clickButtonCheck(CallbackContext &context, double xpos, double ypos, int mouseButton, bool pressed);
	bool cursorMoveButtonCheck(double xpos, double ypos);

private:
	static bool checkClickCollisionForButton(ButtonInfo &button, double xpos, double ypos);
};


#endif //GAME_OF_LIFE_UILAYER_H
