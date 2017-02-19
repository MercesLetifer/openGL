#pragma once

#include "sb7.h"

class main_app : public sb7::application {
	GLuint program_;
	GLuint vao_;

	void compile_shaders();
	void check_shader_errors(GLuint shader, LPCWSTR error_str);

public:
	void startup() override;
	void render(double currentTime) override;
	void shutdown() override;

};