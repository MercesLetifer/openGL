#include "main_app.h"

void main_app::startup()
{
	compile_shaders();

	glCreateVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
}

void main_app::render(double currentTime)
{
	static const GLfloat back_color[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glClearBufferfv(GL_COLOR, NULL, back_color);

	glUseProgram(program_);
	glPointSize(5.0f);					// point radius
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void main_app::shutdown()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteProgram(program_);
}

void main_app::compile_shaders()
{
	static const GLchar* vs_src[] = {
		"#version 450 core																\n"
		"																				\n"
		"const vec4 vertices[3] = vec4[](vec4(-0.25, -0.25, 0.5, 1.0),					\n"
		"								vec4(0.25, 0.25, 0.5, 1.0),						\n"
		"								vec4(0.25, -0.25, 0.5, 1.0));					\n"
		"																				\n"
		"void main(void)																\n"
		"{																				\n"
		"	gl_Position = vertices[gl_VertexID];										\n"
		"}																				\n"
	};

	static const GLchar* fs_src[] = {
		"#version 450 core																\n"
		"																				\n"
		"out vec4 color;																\n"
		"																				\n"
		"void main(void)																\n"
		"{																				\n"
		"	color = vec4(0.0, 0.0, 0.5, 1.0);											\n"
		"}																				\n"
	};

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_src, NULL);
	glCompileShader(vs);
	check_shader_errors(vs, L"Vertex shader source error");

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_src, NULL);
	glCompileShader(fs);
	check_shader_errors(fs, L"Fragment shader source error");

	program_ = glCreateProgram();
	glAttachShader(program_, vs);
	glAttachShader(program_, fs);
	glLinkProgram(program_);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

void main_app::check_shader_errors(GLuint shader, LPCWSTR error_str)
{
	GLint result = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
		MessageBox(NULL, error_str, NULL, MB_OK);
}