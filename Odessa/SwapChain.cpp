#include "pch.h"
#include "SwapChain.h"
#include "Utilities.h"

SwapChain::SwapChain(VkInstance instance, GLFWwindow* window, VkPhysicalDevice GPU) :
	mInstanceRef(instance), glfwWindow(window), mGPURef(GPU)
{
	mSwapchain = VK_NULL_HANDLE;
	mSurface = VK_NULL_HANDLE;

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	mSwapchainWidth = width;
	mSwapchainHeight = height;

	CreateSurface();
}

SwapChain::~SwapChain()
{
	vkDestroySwapchainKHR(mDeviceRef, mSwapchain, nullptr);
	vkDestroySurfaceKHR(mInstanceRef, mSurface, nullptr);
}

void SwapChain::Initialize(VkDevice device)
{
	mDeviceRef = device;

	CreateSwapChain();
	CreateImageViews();
}

VkSurfaceKHR SwapChain::GetSurface()
{
	return VkSurfaceKHR(mSurface);
}

std::vector<VkImageView>* SwapChain::GetImageView()
{
	return &mSwapChainImageViews;
}

void SwapChain::CreateSwapChain()
{
	Utilities::QueueFamilyIndices indices = Utilities::FindQueueFamilies(mGPURef, mSurface);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	VkSwapchainCreateInfoKHR swapchainInfo{};
	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
	swapchainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	swapchainInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
	swapchainInfo.imageExtent = VkExtent2D{ mSwapchainWidth, mSwapchainHeight };
	swapchainInfo.minImageCount = 3;
	swapchainInfo.surface = mSurface;
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
	swapchainInfo.queueFamilyIndexCount = 2;
	swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
	swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainInfo.clipped = VK_TRUE;
	swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(mDeviceRef, &swapchainInfo, nullptr, &mSwapchain) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Swapchain!");
	}

	uint32_t imageCount;
	vkGetSwapchainImagesKHR(mDeviceRef, mSwapchain, &imageCount, nullptr);
	mSwapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(mDeviceRef, mSwapchain, &imageCount, mSwapchainImages.data());
}

void SwapChain::CreateImageViews()
{
	mSwapChainImageViews.resize(mSwapchainImages.size());

	VkImageViewCreateInfo imageViewInfo{};
	imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewInfo.format = VK_FORMAT_B8G8R8A8_SRGB;
	imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewInfo.subresourceRange.baseMipLevel = 0;
	imageViewInfo.subresourceRange.levelCount = 1;
	imageViewInfo.subresourceRange.baseArrayLayer = 0;
	imageViewInfo.subresourceRange.layerCount = 1;

	for (size_t i = 0; i < mSwapchainImages.size(); i++)
	{
		imageViewInfo.image = mSwapchainImages[i];

		if (vkCreateImageView(mDeviceRef, &imageViewInfo, nullptr, &mSwapChainImageViews[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create image views");
		}
	}
}

void SwapChain::CreateSurface()
{
	if (glfwCreateWindowSurface(mInstanceRef, glfwWindow, nullptr, &mSurface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to Create Surface!");
	}
}
