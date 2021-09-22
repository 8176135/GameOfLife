//
// Created by username on 12/09/2021.
//

#ifndef GAME_OF_LIFE_LIFEEXECUTOR_H
#define GAME_OF_LIFE_LIFEEXECUTOR_H

#include "vector"
#include "unordered_map"
#include "unordered_set"
#include "Vector2.h"
#include "cstdint"
#include "RenderWindow.h"

class LifeExecutor {
public:
	explicit LifeExecutor(int reserve_size);
	void next_step();
//	void fill_texture(std::vector<uint8_t> &input_array, const RenderWindow& render_window);
	void randomize_field();

	// Returns true new value was set;
	bool setBit(Vector2<int> pos, bool newValue);

	unsigned long long count();

	const std::unordered_set<Vector2<int>>& live_cells_get();

private:
	std::unordered_map<Vector2<int>, uint8_t> neighbours;
	std::unordered_set<Vector2<int>> live_cells_a;
	std::unordered_set<Vector2<int>> live_cells_b;
	std::unordered_set<Vector2<int>>* live_cells;
	std::unordered_set<Vector2<int>>* last_cells;

};


#endif //GAME_OF_LIFE_LIFEEXECUTOR_H
