// Odessa.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Copyright (c) 2020-2021 Rayvant.

#include "pch.h"
#include "Scene.h"
#include "Types.h"
#include "Camera.h"
#include "Renderer.h"

GLFWwindow* window;
Renderer* renderer;
Camera* mainCamera;
Scene* scene;

bool Keys[1024];

double lastX;
double lastY;

double xChange;
double yChange;

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

	scene = new Scene(renderer);

	while (!glfwWindowShouldClose(window))
	{
		double now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		mainCamera->KeyControl(Keys, (GLfloat)deltaTime);

		mainCamera->CalculateViewMatrix();

		renderer->Render();

		mainCamera->Bind();

		scene->DrawScene();

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

	mainCamera->MouseControl((float)xChange, (float)yChange);
}
