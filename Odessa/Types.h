#pragma once
class Types
{
};

struct Vertex
{
	float Position[3];
	float TexCoord[2];
	float Normal[3];
};

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

