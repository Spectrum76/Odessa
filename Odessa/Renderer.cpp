#include "pch.h"
#include "Renderer.h"

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
