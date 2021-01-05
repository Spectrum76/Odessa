#pragma once
class SwapChain
{
public:
	SwapChain(VkInstance instance, VkSurfaceKHR surface, VkDevice device, VkPhysicalDevice GPU);
	~SwapChain();

	void Initialize();

	VkSwapchainKHR GetSwapChain();

	std::vector<VkImageView>* GetImageView();

protected:
	void CreateSwapChain();
	void CreateImageViews();

private:
	VkSwapchainKHR mSwapchain;

	VkSurfaceKHR mSurfaceRef;

	std::vector<VkImage> mSwapchainImages;
	std::vector<VkImageView> mSwapChainImageViews;

	VkDevice mDeviceRef;

	const VkInstance mInstanceRef;
	const VkPhysicalDevice mGPURef;
};

