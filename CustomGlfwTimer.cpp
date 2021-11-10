// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
//
// Created by username on 12/09/2021.
//

#include "CustomGlfwTimer.h"
#include "GLFW/glfw3.h"

void CustomGlfwTimer::tick() {
	auto currentTime = glfwGetTime();
	for (size_t i = 0; i < intervals.size(); ++i) {
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
