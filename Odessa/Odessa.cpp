// Odessa.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Copyright (c) 2020 Rayvant.

#include "pch.h"
#include "Renderer.h"

GLFWwindow* window;
Renderer* renderer;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Kassandra", nullptr, nullptr);

	renderer = new Renderer(window);

	renderer->InitializeAPI();

	while (!glfwWindowShouldClose(window))
	{
		renderer->Render();
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
