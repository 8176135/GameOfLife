#include "main.h"

#include <cmath>
#include <iostream>
#include <chrono>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include "shader.h"
#include "LifeExecutor.h"
#include "CustomGlfwTimer.h"
#include "RenderWindow.h"

static void lineAlgo(Vector2 start, Vector2 end, const std::function<void(Vector2)> &applicator) {
	auto delta = Vector2(std::abs(end.x - start.x), -std::abs(end.y - start.y));
	auto stepX = start.x < end.x ? 1 : -1;
	auto stepY = start.y < end.y ? 1 : -1;

	auto diff = delta.x + delta.y;

	Vector2 current{start};
	while (true) {
		applicator(current);
		if (current == end) return;
		auto doubleDiff = 2 * diff;
		if (doubleDiff >= delta.y) {
			diff += delta.y;
			current.x += stepX;
		}

		if (doubleDiff <= delta.x) {
			diff += delta.x;
			current.y += stepY;
		}
	}
}

struct CallbackContext {
//	std::vector<uint8_t> textureDataArray;
	LifeExecutor lifeExecutor;
	bool currentlyPlaying;
	RenderWindow renderWindow;
	glm::vec2 movement;
	glm::vec2 lastMousePos;
	double zoomLevel;
};

CallbackContext *get_context(GLFWwindow *w) {
	return static_cast<CallbackContext *>(glfwGetWindowUserPointer(w));
}

static Vector2 convertToOffsetPos(CallbackContext const &context, glm::vec2 toConvert) {
	double mappedXpos = toConvert.x + context.renderWindow.top_left.x;
	double mappedYpos = toConvert.y + context.renderWindow.top_left.y;
	return {(int) std::round(mappedXpos), (int) std::round(mappedYpos)};
}

static std::optional<glm::vec2> getMappedCursorPos(CallbackContext const &context, double xpos, double ypos) {
	if (xpos >= 0 && ypos >= 0 && xpos < WINDOW_SIZE && ypos < WINDOW_SIZE) {
		double mappedXpos = ((xpos / WINDOW_SIZE)) * DEFAULT_BOX_RESOLUTION * context.zoomLevel;
		double mappedYpos = ((ypos / WINDOW_SIZE)) * DEFAULT_BOX_RESOLUTION * context.zoomLevel;
		return glm::vec2(mappedXpos, mappedYpos);
	} else {
		return {};
	}
}

static std::optional<Vector2> getMappedCursorPosOffset(CallbackContext const &context, double xpos, double ypos) {
	auto out = getMappedCursorPos(context, xpos, ypos);
	if (!out.has_value()) {
		return {};
	} else {
		return convertToOffsetPos(context, out.value());
	}
}


namespace Callbacks {
	static void error_callback([[maybe_unused]] int error, const char *description) {
		std::cerr << "Error: " << description << std::endl;
	}

	static void key_callback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action,
							 [[maybe_unused]] int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		CallbackContext *context = get_context(window);

		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			context->lifeExecutor.randomize_field();
//			context->lifeExecutor.fill_texture(context->textureDataArray, context->renderWindow);
		} else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			context->currentlyPlaying = !context->currentlyPlaying;
		} else {
			int change = 0;
			if (action == GLFW_PRESS) {
				change = 1;
			} else if (action == GLFW_RELEASE) {
				change = -1;
			}
			if (key == GLFW_KEY_LEFT) {
				context->movement.x -= change;
			} else if (key == GLFW_KEY_RIGHT) {
				context->movement.x += change;
			} else if (key == GLFW_KEY_DOWN) {
				context->movement.y -= change;
			} else if (key == GLFW_KEY_UP) {
				context->movement.y += change;
			}
		}
	}

	static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
		CallbackContext &context = *get_context(window);
		auto noOffsetMappedPos = getMappedCursorPos(context, xpos, ypos);

		if (!noOffsetMappedPos.has_value()) {
			return;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
			auto convertedPos = convertToOffsetPos(context, noOffsetMappedPos.value());
			auto convertedLastPos = convertToOffsetPos(context, context.lastMousePos);
			lineAlgo(convertedLastPos, convertedPos, [&](Vector2 current) {
				(context.lifeExecutor.setBit(current, true));
			});

		} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
			auto convertedPos = convertToOffsetPos(context, noOffsetMappedPos.value());
			auto convertedLastPos = convertToOffsetPos(context, context.lastMousePos);
			lineAlgo(convertedLastPos, convertedPos, [&](Vector2 current) {
				(context.lifeExecutor.setBit(current, false));
			});
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) {
			if (context.lastMousePos != noOffsetMappedPos) {
				glm::vec2 diff = context.lastMousePos - noOffsetMappedPos.value();
				context.renderWindow += diff;
			}
		}

		context.lastMousePos = noOffsetMappedPos.value();

	}

	static void mouse_button_callback(GLFWwindow *window, int button, int action, [[maybe_unused]] int mods) {
		if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS) {
			CallbackContext &context = *get_context(window);
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			auto mappedPos = getMappedCursorPosOffset(context, xpos, ypos);
			if (!mappedPos.has_value()) {
				return;
			}
			if (context.lifeExecutor.setBit(mappedPos.value(), button == GLFW_MOUSE_BUTTON_LEFT)) {
//				context.lifeExecutor.fill_texture(context.textureDataArray, context.renderWindow);
			}
		}

		if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			CallbackContext &context = *get_context(window);
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			auto mappedPos = getMappedCursorPos(context, xpos, ypos);
			if (!mappedPos.has_value()) {
				return;
			}

			if (action == GLFW_PRESS) {
				context.lastMousePos = mappedPos.value();
			} else if (action == GLFW_RELEASE) {
				context.lastMousePos = {};
			}
		}
	}

	static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
		CallbackContext &context = *get_context(window);
		context.zoomLevel -= yoffset * 0.1;

//		double xpos, ypos;
//		glfwGetCursorPos(window, &xpos, &ypos);

		auto middle = context.renderWindow.get_middle();
		auto offset = glm::vec2((float)(DEFAULT_BOX_RESOLUTION * context.zoomLevel * 0.5));

		context.renderWindow.top_left = middle - offset;
		context.renderWindow.bottom_right = middle + offset;
		std::cout << context.renderWindow.debug() << std::endl;
	}

}

int main() {

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwSetErrorCallback(Callbacks::error_callback);

	CallbackContext context = {
			.lifeExecutor = LifeExecutor(DEFAULT_BOX_RESOLUTION),
			.currentlyPlaying = false,
			.renderWindow = RenderWindow(glm::vec2(), glm::vec2(DEFAULT_BOX_RESOLUTION, DEFAULT_BOX_RESOLUTION)),
			.movement = glm::vec2(),
			.lastMousePos = glm::vec2(0, 0),
			.zoomLevel = 1,
	};

	/* Create a windowed mode window and its OpenGL context */
	glfwWindowHint(GLFW_SAMPLES, 8);
	GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Hello World", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwSetWindowUserPointer(window, &context);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_MULTISAMPLE);

	glfwSwapInterval(1);
	glfwSetKeyCallback(window, Callbacks::key_callback);
	glfwSetMouseButtonCallback(window, Callbacks::mouse_button_callback);
	glfwSetCursorPosCallback(window, Callbacks::cursor_position_callback);
	glfwSetScrollCallback(window, Callbacks::scroll_callback);

	Shader golShader("vertex.glsl", "frag.glsl");

	GLuint vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	const GLfloat vertices[] = {
			// Positions,       Texture Coord
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // top left
	};

	unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
	};

	GLuint vaoId;
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	// This will identify our vertex buffer
	GLuint vertexBufferId, elementBufferId;
	// Generate 1 buffer, put the resulting identifier in vertexBufferId
	glGenBuffers(1, &vertexBufferId);
	// The following commands will talk about our 'vertexBufferId' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0); // NOLINT(mod
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &elementBufferId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//	GLuint mainTexId;
//
//	glGenTextures(1, &mainTexId);
//	glBindTexture(GL_TEXTURE_2D, mainTexId);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = {0.0f, 0.0f, 0.0f, 0.5f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//	generateRandomArray(dataArray, DEFAULT_BOX_RESOLUTION);

//	glGenerateMipmap(GL_TEXTURE_2D);

	golShader.use();
//	golShader.setInt("texture1", 0);
//	golShader.setInt("resolution", DEFAULT_BOX_RESOLUTION);
//    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	int frameCounter = 0;
	CustomGlfwTimer customGlfwTimer;

	customGlfwTimer.register_timer(1, [&] {
		std::cout << "Frames per second: " << frameCounter << std::endl;
		frameCounter = 0;
	});

	std::chrono::duration total_points = std::chrono::duration<long long int, std::nano>::zero();
	int count = 0;

	customGlfwTimer.register_timer(0.025, [&] {
		if (context.currentlyPlaying) {
			auto start = std::chrono::high_resolution_clock::now();
			context.lifeExecutor.next_step();
			auto end = std::chrono::high_resolution_clock::now();
			total_points += end - start;
			count += 1;
		}
	});

	customGlfwTimer.register_timer(0.1, [&] {
		context.renderWindow += context.movement;
		if (context.movement != glm::vec2(0, 0)) {
			if (count != 0) {
				std::cout << total_points.count() / count << "  " << context.lifeExecutor.count() << std::endl;
				count = 0;
				total_points = std::chrono::duration<long long int, std::nano>::zero();
			}
		}
	});

	glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	view = glm::translate(view, glm::vec3(0, 0, -1.0));
	golShader.setMat4("view", view);

	glClearColor(0.2f, 0.3f, 0.2f, 1);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, DEFAULT_BOX_RESOLUTION, DEFAULT_BOX_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE,
//					 context.textureDataArray.data());

//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, mainTexId);

		golShader.use();

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::ortho(context.renderWindow.top_left.x, context.renderWindow.bottom_right.x, context.renderWindow.bottom_right.y, context.renderWindow.top_left.y, 0.1f, 100.0f);

		golShader.setMat4("projection", projection);

		glBindVertexArray(vaoId);
		for (const auto &item : context.lifeExecutor.live_cells_get()) {
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 position = glm::mat4(1.0f);
			position = glm::translate(position, glm::vec3(item.x, item.y, 0));
			golShader.setMat4("position", position);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // NOLINT(mod
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		frameCounter += 1;
		customGlfwTimer.tick();

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}




