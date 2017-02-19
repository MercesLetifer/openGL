#include "main_app.h"

void main_app::startup()
{
}

void main_app::render(double currentTime)
{
	static const GLfloat back_color[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glClearBufferfv(GL_COLOR, NULL, back_color);
}

void main_app::shutdown()
{
}

void main_app::compile_shaders()
{
}

void main_app::check_shader_errors(GLuint shader, LPCWSTR error_str)
{
}