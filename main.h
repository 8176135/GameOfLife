//
// Created by username on 15/09/2021.
//

#ifndef GAME_OF_LIFE_MAIN_H
#define GAME_OF_LIFE_MAIN_H

#include "LifeExecutor.h"
#include "RenderWindow.h"
#include "glm/glm.hpp"
#include "ui/UiLayer.h"
#include <variant>
#include "mutex"

//inline const int DEFAULT_BOX_RESOLUTION = 128;
inline const int DEFAULT_BOX_RESOLUTION = 128;
inline const int WINDOW_SIZE = 1280;

//enum class DrawingMode {
//	Pencil,
//	Box,
//};

namespace DrawingMode {
	struct Pencil {
//		glm::vec2 lastPositionScreen;
	};

	struct Box {
		std::optional<Vector2<int>> startingPos;
		Box() : startingPos({}) {};
	};
}

struct CallbackContext {
	LifeExecutor lifeExecutor;
	bool currentlyPlaying;
	bool wasClicked;
	RenderWindow renderWindow;
	glm::vec2 movement;
	glm::vec2 lastMousePos;
	double zoomLevel;
	std::variant<DrawingMode::Pencil, DrawingMode::Box> currentDrawingMode;
	UiLayer uiLayer;
};

#endif //GAME_OF_LIFE_MAIN_H
