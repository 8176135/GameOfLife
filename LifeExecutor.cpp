//
// Created by username on 12/09/2021.
//

#include "LifeExecutor.h"
#include <cstdint>
#include <random>


static unsigned int seed_val = 101;

static void generateRandomArray(uint8_t *array, unsigned long long size) {
	typedef std::mt19937 MyRngImpl;  // the Mersenne Twister with a popular choice of parameters
	seed_val += 1;
	MyRngImpl rng(seed_val);
	std::uniform_int_distribution<uint8_t> uint_dist(0, 1);

//	auto *dataArray = new uint8_t[128 * 128 * 4];
	std::generate(array, array + size, [&] {
		return uint_dist(rng);
	});

//	return dataArray;
}

LifeExecutor::LifeExecutor(int playground_size) : size(playground_size) {
	playground_a = std::vector<unsigned char>(size * size);
	generateRandomArray(playground_a.data(), playground_a.size());

	playground_b = playground_a;
	current_playground = &playground_a;
	next_playground = &playground_b;
}

void LifeExecutor::next_step() {
	for (int y = 0; y < size; ++y) {
		for (int x = 0; x < size; ++x) {
			unsigned int neighbour_count = 0;
			for (int i1 = -1; i1 < 2; ++i1) {
				for (int i2 = -1; i2 < 2; ++i2) {
					// Better way to do bounds checking?
					if ((i1 == 0 && i2 == 0) || x + i1 >= size || x + i1 < 0 || y + i2 >= size || y + i2 < 0) {
						continue;
					}
					neighbour_count += current_playground->at((y + i2) * size + x + i1);
				}
			}
			auto currentCell = &current_playground->at((y) * size + x);
			if (neighbour_count == 3) {
				next_playground->at((y) * size + x) = true;
			} else if (neighbour_count == 2) {
				next_playground->at((y) * size + x) = *currentCell;
			} else {
				next_playground->at((y) * size + x) = false;
			}
		}
	}

	std::swap(current_playground, next_playground);
}

void LifeExecutor::fill_texture(unsigned char *input_array) {
	for (int i = 0; i < size * size; ++i) {
		uint8_t fill_value = current_playground->at(i) ? 0xFF : 0;
		input_array[i * 4 + 0] = fill_value;
		input_array[i * 4 + 1] = fill_value;
		input_array[i * 4 + 2] = fill_value;
		input_array[i * 4 + 3] = fill_value;
	}
}

void LifeExecutor::randomize_field() {
	generateRandomArray(current_playground->data(), current_playground->size());
}

bool LifeExecutor::setBit(int x, int y, bool newValue) {
	auto valuePtr = &current_playground->at(y * size + x);
	auto oldValue = *valuePtr;
	*valuePtr = newValue;
	return oldValue != newValue;
}
