#pragma once
class Renderer
{
public:
	Renderer();
	~Renderer();

protected:
	void CreateInstance();
	void CreateLogicalDevice();

	void GetPhysicalDevice();

private:
	VkInstance instance;
	VkDevice device;

	VkPhysicalDevice GPU;
};

