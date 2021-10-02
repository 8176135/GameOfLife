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

static void
generateRandomArray(std::unordered_map<Vector2<int>, uint8_t> &toSet, Vector2<int> bounds, int min, int max) {
	typedef std::mt19937 MyRngImpl;  // the Mersenne Twister with a popular choice of parameters
	seed_val += 1;
	MyRngImpl rng(seed_val);
	std::uniform_int_distribution<int> position_gen(0, bounds.x * bounds.y);
	std::uniform_int_distribution<int> range_dist_gen(min, max);
	auto range = range_dist_gen(rng);

	for (int i = 0; i < range; ++i) {
		auto pos = position_gen(rng);
		toSet.emplace(Vector2(pos / bounds.x, pos % bounds.x), 3);
	}
}

LifeExecutor::LifeExecutor(int reserve_size) {
	live_cells_a = std::unordered_map<Vector2<int>, uint8_t>(reserve_size);
	live_cells_b = std::unordered_map<Vector2<int>, uint8_t>(reserve_size);
	live_cells_c = std::unordered_map<Vector2<int>, uint8_t>(reserve_size);

	neighbours = std::unordered_map<Vector2<int>, uint8_t>(reserve_size * 8);

	last_cells = &live_cells_c;
	mid_cells = &live_cells_b;
	live_cells = &live_cells_a;

	generateRandomArray(*live_cells, Vector2(DEFAULT_BOX_RESOLUTION, DEFAULT_BOX_RESOLUTION), 0,
						DEFAULT_BOX_RESOLUTION * DEFAULT_BOX_RESOLUTION);
}

void LifeExecutor::next_step() {
	neighbours.clear();
	std::scoped_lock fl{full_lock};
	for (const auto &item : *live_cells) {

		for (int i1 = -1; i1 < 2; ++i1) {
			for (int i2 = -1; i2 < 2; ++i2) {
				if (i1 == 0 && i2 == 0) {
					continue;
				}

				neighbours[Vector2(item.first.x + i1, item.first.y + i2)] += 1;
			}
		}
	}
	mid_cells->clear();

	for (const auto &item : neighbours) {
		if (item.second == 3) {
			if (live_cells->contains(item.first)) { // If last step had stuff,
				mid_cells->emplace(item.first, 1);
			} else if (last_cells->contains(item.first)) { // If 2 steps ago it had stuff,
				mid_cells->emplace(item.first, 2);
			} else { // Brand new item
				mid_cells->emplace(item.first, 3);
			}
		}

		if (item.second == 2 && live_cells->contains(item.first)) {  // If last step had stuff,
			mid_cells->emplace(item.first, 1);
		}
	}

	{
		std::scoped_lock sl{this->swap_lock};
		// Rotate the cells
		std::swap(live_cells, last_cells
		);
		std::swap(live_cells, mid_cells
		);
	}
}


//void LifeExecutor::fill_texture(std::vector<uint8_t> &input_array, const RenderWindow &render_window) {
//	std::fill(std::begin(input_array), std::end(input_array), '\0');
//	for (const auto &item : *live_cells) {
//		if (item.x < render_window.top_left.x || item.x >= render_window.bottom_right.x ||
//			item.y < render_window.top_left.y || item.y >= render_window.bottom_right.y) {
//			continue;
//		}
//		auto index = ((item.y - render_window.top_left.y) * (render_window.bottom_right.x - render_window.top_left.x) +
//					  (item.x - render_window.top_left.x)) * 4;
//		input_array[index + 0] = 0xFF;
//		input_array[index + 1] = 0xFF;
//		input_array[index + 2] = 0xFF;
//		input_array[index + 3] = 0xFF;
//	}
//}

void LifeExecutor::randomize_field() {
	std::scoped_lock m{full_lock, swap_lock};
	live_cells->clear();
	last_cells->clear();
	generateRandomArray(*live_cells, Vector2(DEFAULT_BOX_RESOLUTION, DEFAULT_BOX_RESOLUTION), 0,
						DEFAULT_BOX_RESOLUTION * DEFAULT_BOX_RESOLUTION);
}

bool LifeExecutor::setBit(Vector2<int> pos, bool newValue) {
	auto retVal = live_cells->contains(pos);
	if (retVal == newValue) {
		return false;
	}

	if (newValue) {
		live_cells->emplace(pos, 3);
	} else {
		live_cells->erase(pos);
	}

	return true;
}

unsigned long long LifeExecutor::count() {
	return live_cells->size();
}

void LifeExecutor::iterate_over_cells(const std::function<void(const std::pair<const Vector2<int>, uint8_t> &)> &to_execute) {
	std::scoped_lock m{swap_lock};

	for (const auto &item : *live_cells) {
		to_execute(item);
	}
}
