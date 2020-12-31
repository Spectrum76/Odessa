#include "pch.h"
#include "Shader.h"

Shader::Shader(VkDevice device, const std::string& filename) : mDeviceRef(device)
{
	mModule = VK_NULL_HANDLE;
}

Shader::~Shader()
{
	vkDestroyShaderModule(mDeviceRef, mModule, nullptr);
}

void Shader::CreateModule()
{
	VkShaderModuleCreateInfo shaderInfo{};
	shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderInfo.codeSize = mCode.size();
	shaderInfo.pCode = reinterpret_cast<const uint32_t*>(mCode.data());

	if (vkCreateShaderModule(mDeviceRef, &shaderInfo, nullptr, &mModule) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create shader module!");
	}
}

void Shader::LoadShaderData(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	mCode = buffer;
}
