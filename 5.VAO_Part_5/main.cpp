#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "string"
#include "iostream"
#include "fstream"
#include "string"
#include "sstream"

typedef unsigned int ui;
using namespace std;

struct shader_source {
	string vertexshader;
	string fragmentshader;
};

shader_source ParseShaders(const string& filename) {
	ifstream streamf(filename);

	if (!streamf.is_open())
		cout << "Error File Open" << endl;

	enum class shadertype {
		none = -1, vertex = 0, fragment = 1
	};

	string line;
	stringstream ss[2];
	shadertype t = shadertype::none;

	while (getline(streamf, line)) {
		if (line.find("#shader") != string::npos) {
			if (line.find("vertex") != string::npos) {
				t = shadertype::vertex;
			}
			else if (line.find("fragment") != string::npos) {
				t = shadertype::fragment;
			}
		}
		else {
			ss[(int)t] << line << '\n';
		}
	}

	return { ss[0].str() , ss[1].str() };
}

ui CompileShader(const string& source, ui type) {
	ui id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		cout << "failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << endl;
		cout << message << endl;
		glDeleteShader(id);
		return 0;

	}
	return id;
}

ui createshader(string& ves, string& fas) {
	ui program = glCreateProgram();
	ui vs = CompileShader(ves, GL_VERTEX_SHADER);
	ui fs = CompileShader(fas, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main() {
	if (!glfwInit())
		return -1;

	GLFWwindow* window;
	window = glfwCreateWindow(640, 480, "VAO_Part_5", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		return -1;

	float triangle1[] = {
		// Позиции точек	Цвет
		-0.75, -0.75,		1.0, 0.0, 0.0,// 0
		-0.75, 0.75,		0.0, 1.0, 0.0, // 1
		- 0.25, 0.75,		0.0, 0.0, 1.0, // 2
		- 0.25, -0.75,		1.0, 1.0, 0.0 // 3
	};

	float triangle2[] = {
		// Позиции точек	Цвет
		0.75, -0.75,		0.0, 0.0, 0.0, // 0
		0.75, 0.75,			1.0, 1.0, 1.0, // 1
		0.25, 0.75,			0.5, 0.5, 0.5, // 2
		0.25, -0.75,		// 3 точка по умолчанию 0.0, 0.0, 0.0
	};

	ui index[] = {
		0,1,2,
		0,2,3
	};

	ui ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Сброс состояния "выбранного" буфера

	ui vaos[2];
	glGenVertexArrays(2, vaos);

	ui buffers[2];
	glGenBuffers(2, buffers);

	glBindVertexArray(vaos[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // IBO
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const void*)(sizeof(float)*2));

	glBindVertexArray(vaos[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // IBO
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const void*)(sizeof(float) * 2));

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Сброс состояния "выбранного" буфера
	glBindVertexArray(0); // Сброс состояния "выбранного" массива вершин

	shader_source shader = ParseShaders("basic.shader");

	cout << "VertexShader\n" << shader.vertexshader << endl;
	cout << "FragmentShader\n" << shader.fragmentshader << endl;

	ui program = createshader(shader.vertexshader, shader.fragmentshader);
	glUseProgram(program);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		for (size_t i = 0; i < 2; i++)
		{
			glBindVertexArray(vaos[i]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteBuffers(2, buffers);
	glDeleteVertexArrays(2, vaos);
	glfwTerminate();
	return 0;
}