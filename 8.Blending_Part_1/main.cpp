#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "monitoring_errors.h"
#include "Renderer.h"
#include "stb_image.h"
#include "Texture.h"

typedef unsigned int ui;
using namespace std;

int main() {
	if (!glfwInit())
		return -1;

	GLFWwindow* window;
	window = glfwCreateWindow(640, 480, "Blending_Part_1", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		return -1;

	float triangle1[] = {
		-0.15, -0.15,
		-0.15, 0.35,
		0.35, 0.35,
		0.35, -0.15,
	};

	float triangle2[] = {
		0.15, 0.15,
		-0.35, 0.15,
		-0.35, -0.35,
		0.15, -0.35,
	};

	ui index[] = {
		0,1,2,
		0,2,3
	};

	IndexBuffer ibo(index, 6);

	VertexBuffer buf1(triangle1, sizeof(triangle1));
	VertexBuffer buf2(triangle2, sizeof(triangle2));

	VertexBufferLayout l1, l2;
	VertexArray v1, v2;

	l1.push<float>(2);
	v1.AddBuffer(buf1, l1);

	l2.push<float>(2);
	v2.AddBuffer(buf2, l2);

	Shader shader("basic.shader");

	Renderer render;

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0, 0, 0, 0);
		render.Clear();

		shader.Bind();
		shader.SetUnifor4f("u_Color", 1.0, 0.0, 0.0, 1.0);
		render.Draw(v1, ibo, shader);
		shader.SetUnifor4f("u_Color", 0.0, 0.0, 1.0, 0.0);
		render.Draw(v2, ibo, shader);
		
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}