#include "main_app.h"
#include <vmath.h>

void main_app::startup()
{
	compile_shaders();

	glCreateVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	
	GLuint vertex_buffer, color_buffer;
	glCreateBuffers(1, &vertex_buffer);
	glCreateBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	
	static const GLfloat vertices[] = {
		-0.25f, -0.25f, 0.5f, 1.0f,
		0.25f, 0.25f, 0.5f, 1.0f,
		0.25f, -0.25f, 0.5f, 1.0f
	};

	static const GLfloat colors[] = {
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	glNamedBufferStorage(vertex_buffer, sizeof(vertices), vertices, GL_MAP_WRITE_BIT);
	glNamedBufferStorage(color_buffer, sizeof(colors), colors, GL_MAP_WRITE_BIT);

	glVertexArrayVertexBuffer(vao_, 0, vertex_buffer, 0, sizeof(vmath::vec4));
	glVertexArrayVertexBuffer(vao_, 1, color_buffer, 0, sizeof(vmath::vec4));

	glVertexArrayAttribFormat(vao_, 0, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vao_, 0, 4, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexArrayAttrib(vao_, 0);
	glEnableVertexArrayAttrib(vao_, 1);
}

void main_app::render(double currentTime)
{
	static const GLfloat back_color[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	glClearBufferfv(GL_COLOR, NULL, back_color);

	glUseProgram(program_);
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
		"layout (location = 0) in vec4 vertex;											\n"
		"layout (location = 1) in vec4 color;											\n"
		"																				\n"
		"out vec4 vs_color;																\n"
		"																				\n"
		"void main(void)																\n"
		"{																				\n"
		"	gl_Position = vertex;														\n"
		"	vs_color = color;															\n"
		"}																				\n"
	};

	static const GLchar* tcs_src[] = {
		"#version 450 core																\n"
		"																				\n"
		"layout (vertices = 3) out;														\n"
		"																				\n"
		"void main(void)																\n"
		"{																				\n"
		"	if (gl_InvocationID == 0) {													\n"
		"		gl_TessLevelInner[0] = 4.0;												\n"
		"		gl_TessLevelOuter[0] = 2.0;												\n"
		"		gl_TessLevelOuter[1] = 2.0;												\n"
		"		gl_TessLevelOuter[2] = 2.0;												\n"
		"	}																			\n"
		"																				\n"
		"	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;	\n"
		"}																				\n"
	};
	
	static const GLchar* tes_src[] = {
		"#version 450 core																\n"
		"																				\n"
		"layout (triangles, equal_spacing, cw) in;										\n"
		"																				\n"
		"void main(void)																\n"
		"{																				\n"
		"	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position +						\n"
		"					gl_TessCoord.y * gl_in[1].gl_Position +						\n"
		"					gl_TessCoord.z * gl_in[2].gl_Position);						\n"
		"}																				\n"
	};

	static const GLchar* gs_src[] = {
		"#version 450 core																\n"
		"																				\n"
		"layout (triangles) in;															\n"
		"layout (points, max_vertices = 3) out;											\n"
		"																				\n"
		"void main(void)																\n"
		"{																				\n"
		"	for (int i = 0; i < gl_in.length(); ++i) {									\n"
		"		gl_Position = gl_in[i].gl_Position;										\n"
		"		EmitVertex();															\n"
		"	}																			\n"
		"}																				\n"
	};

	static const GLchar* fs_src[] = {
		"#version 450 core																\n"
		"																				\n"
		"in vec4 vs_color;																\n"
		"out vec4 color;																\n"
		"																				\n"
		"void main(void)																\n"
		"{																				\n"
		"	color = vs_color;															\n"
		"}																				\n"
	};

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_src, NULL);
	glCompileShader(vs);
	check_shader_errors(vs, L"Vertex shader source error");

//	GLuint tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
//	glShaderSource(tcs, 1, tcs_src, NULL);
//	glCompileShader(tcs);
//	check_shader_errors(tcs, L"Tesselation control shader source error");

//	GLuint tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
//	glShaderSource(tes, 1, tes_src, NULL);
//	glCompileShader(tes);
//	check_shader_errors(tes, L"Tesselation evaluate shader source error");

//	GLuint gs = glCreateShader(GL_GEOMETRY_SHADER);
//	glShaderSource(gs, 1, gs_src, NULL);
//	glCompileShader(gs);
//	check_shader_errors(gs, L"Geometry shader source error");

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_src, NULL);
	glCompileShader(fs);
	check_shader_errors(fs, L"Fragment shader source error");

	program_ = glCreateProgram();
	glAttachShader(program_, vs);
//	glAttachShader(program_, tcs);
//	glAttachShader(program_, tes);
//	glAttachShader(program_, gs);
	glAttachShader(program_, fs);
	glLinkProgram(program_);

	glDeleteShader(vs);
//	glDeleteShader(tcs);
//	glDeleteShader(tes);
//	glDeleteShader(gs);
	glDeleteShader(fs);
}

void main_app::check_shader_errors(GLuint shader, LPCWSTR error_str)
{
	GLint result = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
		MessageBox(NULL, error_str, NULL, MB_OK);
}