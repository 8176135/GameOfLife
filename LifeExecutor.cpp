//
// Created by username on 12/09/2021.
//

#include "LifeExecutor.h"
#include <cstdint>
#include <random>
#include "main.h"

static unsigned int seed_val = 101;

//static void generateRandomArray(uint8_t *array, unsigned long long size) {
//	typedef std::mt19937 MyRngImpl;  // the Mersenne Twister with a popular choice of parameters
//	seed_val += 1;
//	MyRngImpl rng(seed_val);
//	std::uniform_int_distribution<uint8_t> uint_dist(0, 1);
//
////	auto *dataArray = new uint8_t[128 * 128 * 4];
//	std::generate(array, array + size, [&] {
//		return uint_dist(rng);
//	});
//
////	return dataArray;
//}

static void generateRandomArray(std::unordered_set<Vector2> &toSet, Vector2 bounds, int min, int max) {
	typedef std::mt19937 MyRngImpl;  // the Mersenne Twister with a popular choice of parameters
	seed_val += 1;
	MyRngImpl rng(seed_val);
	std::uniform_int_distribution<int> position_gen(0, bounds.x * bounds.y);
	std::uniform_int_distribution<int> range_dist_gen(min, max);
	auto range = range_dist_gen(rng);

	for (int i = 0; i < range; ++i) {
		auto pos = position_gen(rng);
		toSet.insert(Vector2(pos / bounds.x, pos % bounds.x));
	}
}

LifeExecutor::LifeExecutor(int reserve_size) {
	live_cells_a = std::unordered_set<Vector2>(reserve_size);
	live_cells_b = std::unordered_set<Vector2>(reserve_size);
	neighbours = std::unordered_map<Vector2, uint8_t>(reserve_size * 8);

	last_cells = &live_cells_b;
	live_cells = &live_cells_a;

	generateRandomArray(*live_cells, Vector2(BOX_RESOLUTION, BOX_RESOLUTION), 0, BOX_RESOLUTION * BOX_RESOLUTION);
}

void LifeExecutor::next_step() {
	neighbours.clear();
	for (const auto &item : *live_cells) {

		for (int i1 = -1; i1 < 2; ++i1) {
			for (int i2 = -1; i2 < 2; ++i2) {
				if (i1 == 0 && i2 == 0) {
					continue;
				}

				neighbours[Vector2(item.x + i1, item.y + i2)] += 1;
			}
		}
	}
	last_cells->clear();

	for (const auto &item : neighbours) {
		if (item.second == 3 || (item.second == 2 && live_cells->contains(item.first))) {
			last_cells->insert(item.first);
		}
	}

	std::swap(live_cells, last_cells);
}

void LifeExecutor::fill_texture(std::vector<uint8_t> &input_array, const RenderWindow& render_window) {
	std::fill(std::begin(input_array), std::end(input_array), '\0');
	for (const auto &item : *live_cells) {
		if (item.x < render_window.top_left.x || item.x >= render_window.bottom_right.x ||
			item.y < render_window.top_left.y || item.y >= render_window.bottom_right.y) {
			continue;
		}
		auto index = ((item.y - render_window.top_left.y) * (render_window.bottom_right.x - render_window.top_left.x) +
					  (item.x - render_window.top_left.x)) * 4;
		input_array[index + 0] = 0xFF;
		input_array[index + 1] = 0xFF;
		input_array[index + 2] = 0xFF;
		input_array[index + 3] = 0xFF;
	}
}

void LifeExecutor::randomize_field() {
	live_cells->clear();
	generateRandomArray(*live_cells, Vector2(BOX_RESOLUTION, BOX_RESOLUTION), 0, BOX_RESOLUTION * BOX_RESOLUTION);
}

bool LifeExecutor::setBit(int x, int y, bool newValue) {
	auto retVal = live_cells->contains(Vector2(x, y));
	if (retVal == newValue) {
		return false;
	}

	if (newValue) {
		live_cells->insert(Vector2(x, y));
	} else {
		live_cells->erase(Vector2(x, y));
	}

	return true;
}

unsigned long long LifeExecutor::count() {
	return live_cells->size();
}
