#include "main.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <chrono>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "shader.h"
#include "LifeExecutor.h"
#include "CustomGlfwTimer.h"
#include "RenderWindow.h"

struct CallbackContext {
	std::vector<uint8_t> textureDataArray;
	LifeExecutor lifeExecutor;
	bool currentlyPlaying;
	RenderWindow renderWindow;
	Vector2 movement;
};

CallbackContext *get_context(GLFWwindow *w) {
	return static_cast<CallbackContext *>(glfwGetWindowUserPointer(w));
}

static void setLife(CallbackContext &context, double xpos, double ypos, bool newValue) {
	if (xpos >= 0 && ypos >= 0 && xpos < 1280 && ypos < 1280) {
		std::cout << context.renderWindow.debug() << std::endl;
		double mappedXpos = (xpos / 1280) * BOX_RESOLUTION + context.renderWindow.top_left.x;
		double mappedYpos = (1 - (ypos / 1280)) * BOX_RESOLUTION + context.renderWindow.top_left.y;

		if (context.lifeExecutor.setBit((int) std::round(mappedXpos - 0.5), (int) std::round(mappedYpos - 0.5),
										newValue)) {
			context.lifeExecutor.fill_texture(context.textureDataArray, context.renderWindow);
		}
	}
}

namespace Callbacks {
	static void error_callback(int error, const char *description) {
		std::cerr << "Error: " << description << std::endl;
	}

	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		CallbackContext *context = get_context(window);

		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			context->lifeExecutor.randomize_field();
			context->lifeExecutor.fill_texture(context->textureDataArray, context->renderWindow);
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
		CallbackContext *context = get_context(window);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
			setLife(*context, xpos, ypos, true);
		} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
			setLife(*context, xpos, ypos, false);
		}
	}

	static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
		if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS) {
			CallbackContext *context = get_context(window);
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			setLife(*context, xpos, ypos, button == GLFW_MOUSE_BUTTON_LEFT);
		}
	}
}

int main() {

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwSetErrorCallback(Callbacks::error_callback);

	CallbackContext context = {
			.textureDataArray = std::vector<uint8_t>(BOX_RESOLUTION * BOX_RESOLUTION * 4, 0),
			.lifeExecutor = LifeExecutor(BOX_RESOLUTION),
			.currentlyPlaying = false,
			.renderWindow = RenderWindow(Vector2(0, 0), Vector2(BOX_RESOLUTION, BOX_RESOLUTION)),
			.movement = Vector2(0, 0),
	};
	assert(context.textureDataArray.size() == BOX_RESOLUTION * BOX_RESOLUTION * 4);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow *window = glfwCreateWindow(1280, 1280, "Hello World", NULL, NULL);
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

	glfwSwapInterval(1);
	glfwSetKeyCallback(window, Callbacks::key_callback);
	glfwSetMouseButtonCallback(window, Callbacks::mouse_button_callback);
	glfwSetCursorPosCallback(window, Callbacks::cursor_position_callback);

	Shader golShader("vertex.glsl", "frag.glsl");

	GLuint vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	const GLfloat vertices[] = {
			// Positions,       Texture Coord
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top right
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
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

	GLuint mainTexId;

	glGenTextures(1, &mainTexId);
	glBindTexture(GL_TEXTURE_2D, mainTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//	generateRandomArray(dataArray, BOX_RESOLUTION);

//	glGenerateMipmap(GL_TEXTURE_2D);

	golShader.use();
	golShader.setInt("texture1", 0);
	golShader.setInt("resolution", BOX_RESOLUTION);
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
			context.lifeExecutor.fill_texture(context.textureDataArray, context.renderWindow);
		}
	});

	customGlfwTimer.register_timer(0.1, [&] {
		context.renderWindow += context.movement;
		if (context.movement != Vector2(0, 0)) {
			context.lifeExecutor.fill_texture(context.textureDataArray, context.renderWindow);
			if (count != 0) {
				std::cout << total_points.count() / count << "  " << context.lifeExecutor.count() << std::endl;
				count = 0;
				total_points = std::chrono::duration<long long int, std::nano>::zero();
			}
		}

	});

	glClearColor(0.2f, 0.3f, 0.2f, 1);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, BOX_RESOLUTION, BOX_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE,
					 context.textureDataArray.data());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mainTexId);

		golShader.use();
		glBindVertexArray(vaoId);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // NOLINT(mod

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




