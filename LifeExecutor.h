//
// Created by username on 12/09/2021.
//

#ifndef GAME_OF_LIFE_LIFEEXECUTOR_H
#define GAME_OF_LIFE_LIFEEXECUTOR_H

#include <unordered_set>
#include <mutex>
#include "functional"
#include "vector"
#include "unordered_map"
#include "unordered_set"
#include "Vector2.h"
#include "cstdint"
#include "RenderWindow.h"

struct IterateCellParams {
	Vector2<int> coord;
	char8_t freshness;
};

class LifeExecutor {
public:
	explicit LifeExecutor(int reserve_size);
	void next_step();
//	void fill_texture(std::vector<uint8_t> &input_array, const RenderWindow& render_window);
	void randomize_field();

	// Returns true new value was set;
	bool setBit(Vector2<int> pos, bool newValue);

	unsigned long long count();

	void iterate_over_cells(const std::function<void(const IterateCellParams &)>& to_execute);
//	const std::unordered_set<Vector2<int>> &live_cells_get();

	// TODO: Fix this shortcut
	std::mutex full_lock;


private:
	std::unordered_map<Vector2<int>, uint8_t> neighbours;

	std::unordered_map<Vector2<int>, uint8_t> live_cells_a;
	std::unordered_map<Vector2<int>, uint8_t> live_cells_b;
	std::unordered_map<Vector2<int>, uint8_t> live_cells_c;

	std::unordered_map<Vector2<int>, uint8_t>* live_cells;
	std::unordered_map<Vector2<int>, uint8_t>* last_cells;
	std::unordered_map<Vector2<int>, uint8_t>* mid_cells;

	std::mutex swap_lock;
};


#endif //GAME_OF_LIFE_LIFEEXECUTOR_H
