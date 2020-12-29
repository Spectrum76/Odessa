#include "pch.h"
#include "Renderer.h"
#include "Utilities.h"

Renderer::Renderer(GLFWwindow* window) : glfwWindow(window)
{
	instance = VK_NULL_HANDLE;
	device = VK_NULL_HANDLE;
	GPU = VK_NULL_HANDLE;
	graphicsQueue = VK_NULL_HANDLE;
	presentQueue = VK_NULL_HANDLE;
	surface = VK_NULL_HANDLE;
}

Renderer::~Renderer()
{
	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);
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
