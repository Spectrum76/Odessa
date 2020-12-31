#pragma once
#include "SwapChain.h"
#include "RenderPass.h"

class Renderer
{
public:
	Renderer(GLFWwindow* window);
	~Renderer();

	void InitializeAPI();

protected:
	void CreateInstance();
	void CreateLogicalDevice();

	void GetPhysicalDevice();

private:
	VkInstance instance;
	VkDevice device;

	GLFWwindow* glfwWindow;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkPhysicalDevice GPU;

	std::unique_ptr<SwapChain> mSwapChain;
	std::unique_ptr<RenderPass> mRenderPass;
};
