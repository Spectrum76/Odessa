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
	void CreateSurface();

	void GetPhysicalDevice();

private:
	VkInstance instance;
	VkDevice device;

	GLFWwindow* glfwWindow;
	VkSurfaceKHR surface;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkPhysicalDevice GPU;

	size_t mCurrentFrame;

	std::vector<VkSemaphore> imageAvailableSemaphore;
	std::vector<VkSemaphore> renderFinishedSemaphore;

	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	std::unique_ptr<SwapChain> mSwapChain;
	std::unique_ptr<RenderPass> mRenderPass;
	std::unique_ptr<CommandBuffer> mCommandBuffer;
};

