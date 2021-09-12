//
// Created by username on 12/09/2021.
//

#ifndef GAME_OF_LIFE_LIFEEXECUTOR_H
#define GAME_OF_LIFE_LIFEEXECUTOR_H

#include "vector"

class LifeExecutor {
public:
	explicit LifeExecutor(int size);
	void next_step();
	void fill_texture(unsigned char *input_array);
	void randomize_field();

	// Returns true new value was set;
	bool setBit(int x, int y, bool newValue);

private:
	int size;
	std::vector<unsigned char> playground_a;
	std::vector<unsigned char> playground_b;
	std::vector<unsigned char>* current_playground;
	std::vector<unsigned char>* next_playground;

};


#endif //GAME_OF_LIFE_LIFEEXECUTOR_H
