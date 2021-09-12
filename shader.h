//
// Created by username on 11/09/2021.
//

#ifndef GAME_OF_LIFE_SHADER_H
#define GAME_OF_LIFE_SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	// the program ID
	unsigned int ID;

	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string &name, bool value);
	void setInt(const std::string &name, int value);
	void setFloat(const std::string &name, float value);

	Shader(const Shader& original) = delete;
	Shader& operator=(const Shader& original) = delete;

	virtual ~Shader();
};


#endif //GAME_OF_LIFE_SHADER_H
