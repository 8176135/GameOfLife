//
// Created by username on 13/09/2021.
//

#ifndef GAME_OF_LIFE_VECTOR2_H
#define GAME_OF_LIFE_VECTOR2_H

#include <string>
#include <sstream>
#include "unordered_set"

struct Vector2 {
	int x, y;

	Vector2(int x, int y) : x(x), y(y) {};

	bool operator==(const Vector2 &other) const {
		return (x == other.x
				&& y == other.y);
	}

	Vector2 &operator+=(const Vector2 &rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	[[nodiscard]] std::string debug() const {
		std::ostringstream stream;
		stream << "Vector2(" << this->x << ", " << this->y << ")";
		return stream.str();
	}

};

inline Vector2 operator+(Vector2 lhs, const Vector2 &rhs) {
	lhs += rhs;
	return lhs;
}

namespace std {
	template<>
	struct hash<Vector2> {
		std::size_t operator()(const Vector2 &k) const {
			using std::size_t;
			using std::hash;

			return k.x * 1735499 + k.y * 7436369;
		}
	};

}
#endif //GAME_OF_LIFE_VECTOR2_H
