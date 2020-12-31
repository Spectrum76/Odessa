#pragma once
class SwapChain
{
public:
	SwapChain(VkInstance instance, GLFWwindow* window, VkPhysicalDevice GPU);
	~SwapChain();

	void Initialize(VkDevice device);

	VkSurfaceKHR GetSurface();
	std::vector<VkImageView>* GetImageView();

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

	VkDevice mDeviceRef;

	const VkInstance mInstanceRef;
	const VkPhysicalDevice mGPURef;

	uint32_t mSwapchainWidth;
	uint32_t mSwapchainHeight;
};

