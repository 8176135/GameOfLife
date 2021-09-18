//
// Created by username on 14/09/2021.
//

#ifndef GAME_OF_LIFE_RENDERWINDOW_H
#define GAME_OF_LIFE_RENDERWINDOW_H

//#include "Vector2.h"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "string"

using glm::vec2;

struct RenderWindow {
	RenderWindow(const vec2 &topLeft, const vec2 &bottomRight) : top_left(topLeft), bottom_right(bottomRight) {}

	vec2 top_left;
	vec2 bottom_right;

	vec2 get_middle() {
		return (this->bottom_right - this->top_left) * glm::vec2(0.5) + this->top_left;
	}

	RenderWindow &operator+=(const vec2 &rhs) {
		this->bottom_right += rhs;
		this->top_left += rhs;
		return *this;
	}

	RenderWindow operator + (const vec2 &rhs) {
		RenderWindow other(*this);
		other += rhs;
		return other;
	}

	[[nodiscard]] std::string debug() const {
		std::ostringstream output;
		output << glm::to_string(this->top_left) << " , " << glm::to_string(this->bottom_right) << ";";
		return output.str();
	}
};

#endif //GAME_OF_LIFE_RENDERWINDOW_H
