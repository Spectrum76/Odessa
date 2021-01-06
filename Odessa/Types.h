#pragma once
class Types
{
};

struct Vertex
{
	float position[3];
	float texCoord[2];
	float normal[3];
};

struct CameraUBO
{
	glm::mat4 View;
	glm::mat4 Proj;
	glm::vec3 Eye;
};

