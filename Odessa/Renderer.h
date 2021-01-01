#pragma once
#include "SwapChain.h"
#include "RenderPass.h"
#include "CommandBuffer.h"

class Renderer
{
public:
	Renderer(GLFWwindow* window);
	~Renderer();

	void Render();
	void InitializeAPI();

protected:
	void CreateInstance();
	void CreateLogicalDevice();
	void CreateSemaphores();

	void GetPhysicalDevice();

private:
	VkInstance instance;
	VkDevice device;

	GLFWwindow* glfwWindow;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkPhysicalDevice GPU;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

	std::unique_ptr<SwapChain> mSwapChain;
	std::unique_ptr<RenderPass> mRenderPass;
	std::unique_ptr<CommandBuffer> mCommandBuffer;
};

