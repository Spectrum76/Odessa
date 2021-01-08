// Odessa.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Copyright (c) 2020-2021 Rayvant.

#include "pch.h"
#include "Camera.h"
#include "Renderer.h"

GLFWwindow* window;
Renderer* renderer;
Camera* mainCamera;

bool Keys[1024];

float lastX;
float lastY;

float xChange;
float yChange;

bool mouseFirstMoved = true;

double deltaTime = 0.0f;
double lastTime = 0.0f;

void HandleKeys(GLFWwindow* window, int Key, int Code, int Action, int Mode);
void HandleMouse(GLFWwindow* window, double xPos, double yPos);

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Kassandra", nullptr, nullptr);

	glfwSetKeyCallback(window, HandleKeys);
	glfwSetCursorPosCallback(window, HandleMouse);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	renderer = new Renderer(window);

	renderer->Init();

	mainCamera = new Camera(renderer);

	while (!glfwWindowShouldClose(window))
	{
		renderer->Render();
		renderer->Present();
		glfwPollEvents();
	}

	delete mainCamera;

	delete renderer;

	glfwDestroyWindow(window);
	glfwTerminate();
}

void HandleKeys(GLFWwindow* window, int Key, int Code, int Action, int Mode)
{
	if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (Key >= 0 && Key < 1024)
	{
		if (Action == GLFW_PRESS)
		{
			Keys[Key] = true;
		}
		else if (Action == GLFW_RELEASE)
		{
			Keys[Key] = false;
		}
	}
}

void HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	if (mouseFirstMoved)
	{
		lastX = xPos;
		lastY = yPos;
		mouseFirstMoved = false;
	}

	xChange = xPos - lastX;
	yChange = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	mainCamera->MouseControl(xChange, yChange);
}
