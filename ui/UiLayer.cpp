//
// Created by username on 22/09/2021.
//

#include <glm/ext/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include "glad/glad.h"
#include "UiLayer.h"
#include "../main.h"

UiLayer::UiLayer() : uiShader(Shader("ui/vertex_ui.glsl", "ui/frag_ui.glsl")) {
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(-1, 1, 0));
	transform = glm::scale(transform, glm::vec3(0.1, 0.1, 1));
	transform = glm::translate(transform, glm::vec3(0.5, -0.5, 0));
	this->elementTransforms.push_back(transform);
	this->buttonCallbacks.emplace_back([](CallbackContext& context) {
		if (std::holds_alternative<DrawingMode::Box>(context.currentDrawingMode)) {
			context.currentDrawingMode = DrawingMode::Pencil{};
		} else if (std::holds_alternative<DrawingMode::Pencil>(context.currentDrawingMode)) {
			context.currentDrawingMode = DrawingMode::Box();
		}
//		std::cout << "YOLO" << std::endl;
	});
}

void UiLayer::RenderLayer() {
	this->uiShader.use();
	// TODO: Separate vertex array

	for (const auto &item : this->elementTransforms) {
		uiShader.setMat4("transform", item);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // NOLINT(mod
	}
}

bool UiLayer::checkClickCollision(CallbackContext& context, double xpos, double ypos) {
	glm::vec2 mappedPos((xpos / WINDOW_SIZE) * 2 - 1, 1 - (ypos / WINDOW_SIZE) * 2);

	bool clicked = false;
	for (unsigned long long i = 0; i < this->elementTransforms.size(); ++i) {
		uiShader.setMat4("transform", this->elementTransforms[i]);
		glm::vec4 positionTopLeft(0.5f, 0.5f, 0, 1);
		glm::vec4 positionBottomRight = positionTopLeft;
		positionBottomRight.x *= -1;
		positionBottomRight.y *= -1;

		positionTopLeft = this->elementTransforms[i] * positionTopLeft;
		positionBottomRight = this->elementTransforms[i] * positionBottomRight;

		if (positionTopLeft.x > mappedPos.x &&
			positionBottomRight.x < mappedPos.x &&
			positionTopLeft.y > mappedPos.x &&
			positionBottomRight.y < mappedPos.y) {

			clicked = true;
			buttonCallbacks[i](context);
		}
	}
	return clicked;
}
