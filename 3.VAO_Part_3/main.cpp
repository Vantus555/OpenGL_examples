#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
	window = glfwCreateWindow(640, 480, "VAO_Part_3", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		return -1;

	float triangle1[] = {
		-1.0, -1.0,
		-1.0, 1.0,
		0.0, 1.0
	};

	float triangle2[] = {
		1.0, -1.0,
		1.0, 1.0,
		0.0, 1.0
	};

	ui vaos[2];
	glGenVertexArrays(2, vaos);

	ui buffers[2];
	glGenBuffers(2, buffers);

	glBindVertexArray(vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	glBindVertexArray(vaos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // —брос состо€ни€ "выбранного" буфера
	glBindVertexArray(0); // —брос состо€ни€ "выбранного" массива вершин

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
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}