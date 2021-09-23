//
// Created by username on 22/09/2021.
//

#include <glm/ext/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "UiLayer.h"
#include "../main.h"

UiLayer::UiLayer() : uiShader(Shader("ui/vertex_ui.glsl", "ui/frag_ui.glsl")) {
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(-1, 1, 0));
	transform = glm::scale(transform, glm::vec3(0.1, 0.1, 1));
	transform = glm::translate(transform, glm::vec3(0.5, -0.5, 0));

	this->buttons.push_back(ButtonInfo{
			.elementTransform = transform,
			.callback = [](CallbackContext &context) {
				if (std::holds_alternative<DrawingMode::Box>(context.currentDrawingMode)) {
					context.currentDrawingMode = DrawingMode::Pencil{};
				} else if (std::holds_alternative<DrawingMode::Pencil>(context.currentDrawingMode)) {
					context.currentDrawingMode = DrawingMode::Box{};
				}
				//		std::cout << "YOLO" << std::endl;
			},
			.currentState = ButtonState::None,
	});
}

void UiLayer::RenderLayer() {
	this->uiShader.use();
	// TODO: Separate vertex array

	for (const auto &item : this->buttons) {
		uiShader.setMat4("transform", item.elementTransform);
		glm::vec3 tintVal;
		switch (item.currentState) {
			case ButtonState::Hover:
				tintVal = glm::vec3(0.8);
				break;
			case ButtonState::Active:
				tintVal = glm::vec3(0.5);
				break;
			case ButtonState::None:
				tintVal = glm::vec3(1);
				break;
		}

		uiShader.setVec3("tint", tintVal);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // NOLINT(mod
	}
}

bool UiLayer::clickButtonCheck(CallbackContext &context, double xpos, double ypos, int mouseButton, bool pressed) {
	bool clicked = false;

	for (auto &button : this->buttons) {
		// TODO: Don't really like how this is layed out, bit of a mess.
		// Was wasPressed should not be used if pressed is true.
		bool wasPressed;
		if (!pressed) {
			wasPressed = button.mouseButtonDown.erase(mouseButton) > 0;
			std::cout << wasPressed << std::endl;
		}

		if (!checkClickCollisionForButton(button, xpos, ypos)) {
			if (button.mouseButtonDown.empty()) { // Not on the button
				button.currentState = ButtonState::None;
			}
			continue;
		}
		clicked = true;

		if (pressed) {
			button.mouseButtonDown.insert(mouseButton);
			button.currentState = ButtonState::Active;
		} else if (wasPressed) {
			button.callback(context);
			if (button.mouseButtonDown.empty()) {
				// Still on the button
				button.currentState = ButtonState::Hover;
			}
		}
	}
	return clicked;
}

bool UiLayer::cursorMoveButtonCheck(double xpos, double ypos) {
	for (auto &button : this->buttons) {
		if (button.mouseButtonDown.empty()) {
			button.currentState = checkClickCollisionForButton(button, xpos, ypos) ? ButtonState::Hover
																				   : ButtonState::None;
		}

	}
	return false;
}

bool UiLayer::checkClickCollisionForButton(ButtonInfo &button, double xpos, double ypos) {
	glm::vec2 mappedPos((xpos / WINDOW_SIZE) * 2 - 1, 1 - (ypos / WINDOW_SIZE) * 2);
	glm::vec4 positionTopLeft(0.5f, 0.5f, 0, 1);
	glm::vec4 positionBottomRight = positionTopLeft;
	positionBottomRight.x *= -1;
	positionBottomRight.y *= -1;

	positionTopLeft = button.elementTransform * positionTopLeft;
	positionBottomRight = button.elementTransform * positionBottomRight;

	return (positionTopLeft.x > mappedPos.x &&
			positionBottomRight.x < mappedPos.x &&
			positionTopLeft.y > mappedPos.x &&
			positionBottomRight.y < mappedPos.y);
}

