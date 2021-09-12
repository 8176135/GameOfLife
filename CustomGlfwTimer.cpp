//
// Created by username on 12/09/2021.
//

#include "CustomGlfwTimer.h"
#include "GLFW/glfw3.h"

void CustomGlfwTimer::tick() {
	auto currentTime = glfwGetTime();
	for (unsigned int i = 0; i < intervals.size(); ++i) {
		auto interval = std::get<0>(intervals[i]);
		if (oldTimes[i] + interval < currentTime) {
			oldTimes[i] = currentTime;
			std::get<1>(intervals[i])();
		}
	}
}

void CustomGlfwTimer::register_timer(double interval, const std::function<void()> &lambda) {
	intervals.emplace_back(interval, lambda);
	oldTimes.push_back(glfwGetTime());
}
