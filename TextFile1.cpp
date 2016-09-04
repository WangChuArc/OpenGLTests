#pragma once
#include<stdlib.h>
#define GLEW_STATIC
#include"glew.h"
#include<glfw3.h>
#include<functional>
#include<vector>
#include<iostream>
#include "shaderHandle.h"

GLFWwindow* pWindow;

int main(void)
{
	// 定义清理函数
	typedef std::vector<std::function<void()>> FuncVec;
	FuncVec cleanUpFuncs;
	auto cleanUp = [&cleanUpFuncs]()->void
	{
		for (FuncVec::iterator it = cleanUpFuncs.begin(); it != cleanUpFuncs.end(); it++) (*it)();
	};

	// 初始化
	if (!glfwInit())
	{
		std::cout << "fail to init \"glfw\"" << std::endl;
		cleanUp();
		return -1;
	}
	else
	{
		cleanUpFuncs.push_back(std::move(std::function<void()>(glfwTerminate)));
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	pWindow = glfwCreateWindow(1024, 768, "An OpenGL window", NULL, NULL);
	if (nullptr == pWindow)
	{
		std::cout << "fail to create a window" << std::endl;
		cleanUp();
		int a;
		std::cin >> a;
		return -1;
	}

	glfwMakeContextCurrent(pWindow);
    glewExperimental = true;
	if (GLEW_OK != glewInit())
	{
		std::cout << "fail to init \"glew\"" << std::endl;
		cleanUp();
		return -1;
	}

	glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	GLfloat vertexs[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_VERTEX_ARRAY, VBO);
	glBufferData(GL_VERTEX_ARRAY, sizeof(GLfloat) * 6, (void*)vertexs, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, nullptr);

	do {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	} while (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(pWindow) == 0);
    shaderHandle::shader shader1;
	cleanUp();
	return 0;
}