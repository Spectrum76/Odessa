#pragma once
class Types
{
};

class Vertex
{
public:
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;

	bool operator==(const Vertex& other) const;
};

namespace std
{
	template<> struct hash<Vertex>
	{
		size_t operator()(Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.Position) ^
				(hash<glm::vec2>()(vertex.TexCoord) << 1)) >> 1) ^
				(hash<glm::vec3>()(vertex.Normal) << 1);
		}
	};
}

struct CameraUBO
{
	glm::mat4 View;
	glm::mat4 Proj;
	glm::vec3 Eye;
};

struct Transform
{
	glm::mat4 FModel;
	glm::mat4 iModel;
};

