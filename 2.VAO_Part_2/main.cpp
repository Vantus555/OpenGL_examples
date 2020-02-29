#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef unsigned int ui;

int main() {
	if (!glfwInit())
		return -1;

	GLFWwindow* window;
	window = glfwCreateWindow(640, 480, "VAO_Part_2", NULL, NULL);
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