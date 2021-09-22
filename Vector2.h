//
// Created by username on 13/09/2021.
//

#ifndef GAME_OF_LIFE_VECTOR2_H
#define GAME_OF_LIFE_VECTOR2_H

#include <string>
#include <sstream>
#include <iostream>
#include "unordered_set"
#include <cmath>
#include "concepts"

template <typename T>
struct Vector2 {
	T x, y;

	Vector2(T x, T y) : x(x), y(y) {};
	Vector2(T scalar) : x(scalar), y(scalar) {};

//	Vector2(const Vector2& input) : x(input.x), y(input.y) {
//		std:: cout << "YOLO" << std::endl;
//	}

	static Vector2 zero() {
		return {0, 0};
	}

	bool operator==(const Vector2 &other) const {
		return (x == other.x
				&& y == other.y);
	}

	Vector2 &operator+=(const Vector2 &rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	Vector2 &operator-=(const Vector2 &rhs) {
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	Vector2 abs() {
		return Vector2(std::abs(this->x), std::abs(this->y));
	}

	[[nodiscard]] std::string debug() const {
		std::ostringstream stream;
		stream << "Vector2(" << this->x << ", " << this->y << ")";
		return stream.str();
	}

private:
	friend std::ostream& operator<<(std::ostream& os, Vector2 const& rhs) {
		os << "Vector2(" << rhs.x << ", " << rhs.y << ")";
		return os;
	}
};

template <typename T>
inline Vector2<T> operator+(Vector2<T> lhs, const Vector2<T> &rhs) {
	lhs += rhs;
	return lhs;
}

template <typename T>
inline Vector2<T> operator-(Vector2<T> lhs, const Vector2<T> &rhs) {
	lhs -= rhs;
	return lhs;
}

namespace std {
	template<typename T>
	struct hash<Vector2<T>> {
		std::size_t operator()(const Vector2<T> &k) const {
			using std::size_t;
			using std::hash;

			return k.x * 1735499 + k.y * 7436369;
		}
	};

}
#endif //GAME_OF_LIFE_VECTOR2_H
