// Odessa.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Copyright (c) 2020 Rayvant.

#include "pch.h"
#include "Renderer.h"

GLFWwindow* window;
Renderer* renderer;

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

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
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
