//
// Created by username on 12/09/2021.
//

#ifndef GAME_OF_LIFE_CUSTOMGLFWTIMER_H
#define GAME_OF_LIFE_CUSTOMGLFWTIMER_H

#include <functional>
#include "vector"

class CustomGlfwTimer {
public:
	void register_timer(double interval, const std::function< void () >& lambda);
	void tick();

private:
	std::vector<std::tuple<double, std::function< void () >>> intervals;
	std::vector<double> oldTimes;
};


#endif //GAME_OF_LIFE_CUSTOMGLFWTIMER_H
