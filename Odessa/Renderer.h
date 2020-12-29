#pragma once
class Renderer
{
public:
	Renderer(GLFWwindow* window);
	~Renderer();

	void InitializeAPI();

protected:
	void CreateInstance();
	void CreateLogicalDevice();
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
};

