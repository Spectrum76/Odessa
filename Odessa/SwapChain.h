#pragma once
class SwapChain
{
public:
	SwapChain(VkInstance instance, VkDevice device, GLFWwindow* window, VkPhysicalDevice GPU);
	~SwapChain();

	void Initialize();

	VkSurfaceKHR GetSurface();

protected:
	void CreateSwapChain();
	void CreateImageViews();
	void CreateSurface();

private:
	VkSwapchainKHR mSwapchain;

	GLFWwindow* glfwWindow;
	VkSurfaceKHR mSurface;

	std::vector<VkImage> mSwapchainImages;
	std::vector<VkImageView> mSwapChainImageViews;

	const VkInstance mInstanceRef;
	const VkDevice   mDeviceRef;
	const VkPhysicalDevice mGPURef;

	uint32_t mSwapchainWidth;
	uint32_t mSwapchainHeight;
};

