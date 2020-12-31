#pragma once
class Shader
{
public:
	Shader(VkDevice device, const std::string& filename);
	~Shader();

	VkShaderModule GetShaderModule();

protected:
	void CreateModule();
	void LoadShaderData(const std::string& filename);

private:
	VkShaderModule mModule;

	const VkDevice mDeviceRef;

	std::vector<char> mCode;
};

