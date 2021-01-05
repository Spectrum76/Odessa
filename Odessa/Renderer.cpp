#include "pch.h"
#include "Renderer.h"
#include "Utilities.h"

Renderer::Renderer(GLFWwindow* window) : glfwWindow(window)
{
	instance = VK_NULL_HANDLE;
	device = VK_NULL_HANDLE;
	GPU = VK_NULL_HANDLE;

	surface = VK_NULL_HANDLE;

	graphicsQueue = VK_NULL_HANDLE;
	presentQueue = VK_NULL_HANDLE;

	mCurrentFrame = 0;
}

Renderer::~Renderer()
{
	vkDeviceWaitIdle(device);

	for (size_t i = 0; i < InFlightCount; i++)
	{
		vkDestroySemaphore(device, renderFinishedSemaphore[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphore[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

	mCommandBuffer.reset();

	mRenderPass.reset();

	mSwapChain.reset();

	vkDestroySurfaceKHR(instance, surface, nullptr);

	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(instance, nullptr);
}

void Renderer::Render()
{
	vkWaitForFences(device, 1, &inFlightFences[mCurrentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, mSwapChain->GetSwapChain(), UINT64_MAX, imageAvailableSemaphore[mCurrentFrame], VK_NULL_HANDLE, &imageIndex);

	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
	{
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}

	imagesInFlight[imageIndex] = inFlightFences[mCurrentFrame];

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore[mCurrentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	VkCommandBuffer commandBuffer = (*(mCommandBuffer->GetCommandBuffer()))[imageIndex];

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore[mCurrentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &inFlightFences[mCurrentFrame]);

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[mCurrentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { mSwapChain->GetSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(presentQueue, &presentInfo);

	mCurrentFrame = (mCurrentFrame + 1) % InFlightCount;
}

void Renderer::InitializeAPI()
{
	CreateInstance();
	GetPhysicalDevice();

	CreateSurface();

	CreateLogicalDevice();

	CreateSemaphores();

	mSwapChain = std::make_unique<SwapChain>(instance, surface, device, GPU);

	mSwapChain->Initialize();

	mRenderPass = std::make_unique<RenderPass>(device, mSwapChain->GetImageView());

	mRenderPass->Initialize();

	mCommandBuffer = std::make_unique<CommandBuffer>(device, surface, GPU);

	mCommandBuffer->Initialize();

	mCommandBuffer->Record(mRenderPass->GetRenderPass(), mRenderPass->GetFramebuffer());
}

void Renderer::CreateInstance()
{
	VkApplicationInfo appInfo{};
	VkInstanceCreateInfo instanceInfo{};

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "KassandraFx";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Kassandra Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	instanceInfo.enabledExtensionCount = glfwExtensionCount;
	instanceInfo.ppEnabledExtensionNames = glfwExtensions;
	instanceInfo.enabledLayerCount = 0;

#ifdef _DEBUG

	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	instanceInfo.enabledLayerCount = (uint32_t)validationLayers.size();
	instanceInfo.ppEnabledLayerNames = validationLayers.data();

#endif // DEBUG

	if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to Create Instance!");
	}
}

void Renderer::CreateLogicalDevice()
{
	Utilities::QueueFamilyIndices queueIndex = Utilities::FindQueueFamilies(GPU, surface);
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { queueIndex.graphicsFamily.value(), queueIndex.presentFamily.value() };

	std::vector<const char*> deviceExtension = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(GPU, &deviceFeatures);

	VkDeviceCreateInfo deviceInfo{};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceInfo.pEnabledFeatures = &deviceFeatures;
	deviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtension.size());
	deviceInfo.ppEnabledExtensionNames = deviceExtension.data();

	if (vkCreateDevice(GPU, &deviceInfo, nullptr, &device) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device!");
	}

	vkGetDeviceQueue(device, queueIndex.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, queueIndex.presentFamily.value(), 0, &presentQueue);
}

void Renderer::CreateSemaphores()
{
	imageAvailableSemaphore.resize(InFlightCount);
	renderFinishedSemaphore.resize(InFlightCount);

	inFlightFences.resize(InFlightCount);
	imagesInFlight.resize(ImageCount, VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < InFlightCount; i++)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create semaphores!");
		}
	}
}

void Renderer::CreateSurface()
{
	if (glfwCreateWindowSurface(instance, glfwWindow, nullptr, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to Create Surface!");
	}
}

void Renderer::GetPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	std::vector <VkPhysicalDevice> DevList(deviceCount);

	vkEnumeratePhysicalDevices(instance, &deviceCount, DevList.data());

	for (const auto& device : DevList)
	{
		GPU = device;
	}

	VkPhysicalDeviceProperties deviceProperties;

	vkGetPhysicalDeviceProperties(GPU, &deviceProperties);

	uint32_t apiVer = deviceProperties.apiVersion;

	std::cout << "Name: " << deviceProperties.deviceName << std::endl;
	std::cout << "Vulkan API Version: "
		<< VK_VERSION_MAJOR(apiVer) << "."
		<< VK_VERSION_MINOR(apiVer) << "."
		<< VK_VERSION_PATCH(apiVer) << std::endl;
}
