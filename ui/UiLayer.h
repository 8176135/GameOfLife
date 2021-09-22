//
// Created by username on 22/09/2021.
//

#ifndef GAME_OF_LIFE_UILAYER_H
#define GAME_OF_LIFE_UILAYER_H

#include "../main.fwd.h"
#include <vector>
#include <functional>
#include "../shader.h"

class UiLayer {
	Shader uiShader;
	std::vector<glm::mat4> elementTransforms;
	std::vector<std::function<void (CallbackContext&) >> buttonCallbacks;

public:
	UiLayer();
	void RenderLayer();
	/// Returns true if hit a ui element, so stop propagating.
	bool checkClickCollision(CallbackContext& context, double xpos, double ypos);
};


#endif //GAME_OF_LIFE_UILAYER_H
