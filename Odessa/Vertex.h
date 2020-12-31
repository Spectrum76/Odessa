#pragma once
class Vertex
{
public:
	glm::vec2 Position;
	glm::vec3 Color;

	static VkVertexInputBindingDescription GetBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
};

