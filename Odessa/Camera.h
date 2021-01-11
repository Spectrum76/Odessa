#pragma once
#include "Types.h"
#include "Renderer.h"

class Camera
{
public:
	Camera(Renderer* renderer);

	Camera(glm::vec3 startPosition, glm::vec3 startUp,
		float startYaw, float startPitch,
		float startMoveSpeed, float startTurnSpeed,
		ID3D11Device* device, ID3D11DeviceContext* context);

	void Bind();

	void KeyControl(bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange);

	void CalculateViewMatrix();
	
	~Camera();

protected:
	void Update();
	void CreateUBO();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	ID3D11Device* mDeviceRef;
	ID3D11DeviceContext* mDeviceContextRef;

	ID3D11Buffer* mUniformBuffer;

	CameraUBO __Data;

	float yaw;
	float pitch;

	float moveSpeed;
	float turnSpeed;
};

