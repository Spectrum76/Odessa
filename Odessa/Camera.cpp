#include "pch.h"
#include "Types.h"
#include "Camera.h"
#include "Renderer.h"

Camera::Camera(Renderer* renderer) : Camera(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f),
	-90.0f, 0.0f, 5.0f, 0.2f, renderer->GetDevice(), renderer->GetContext())
{
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp,
	float startYaw, float startPitch,
	float startMoveSpeed, float startTurnSpeed,
	ID3D11Device* device, ID3D11DeviceContext* context) : mDeviceRef(device), mDeviceContextRef(context)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	__Data.View = glm::mat4(1.0f);
	__Data.Proj = glm::mat4(1.0f);

	__Data.Proj = glm::perspectiveFov(glm::radians(45.0f), (float)WIDTH, (float)HEIGHT, 0.1f, 1000.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	Update();
	CreateUBO();
}

void Camera::Bind()
{
	mDeviceContextRef->VSSetConstantBuffers(1, 1, &mUniformBuffer);
}

void Camera::Bind(ID3D11DeviceContext* DefCtx)
{
	DefCtx->VSSetConstantBuffers(1, 1, &mUniformBuffer);
}

void Camera::KeyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	Update();
}

void Camera::CalculateViewMatrix()
{
	__Data.View = glm::lookAt(position, position + front, up);
	__Data.Eye = position;

	D3D11_MAPPED_SUBRESOURCE cbRes;
	mDeviceContextRef->Map(mUniformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbRes);
	CopyMemory(cbRes.pData, &__Data, sizeof(CameraUBO));
	mDeviceContextRef->Unmap(mUniformBuffer, 0);
}

Camera::~Camera()
{
	mUniformBuffer->Release();
}

void Camera::Update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::CreateUBO()
{
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));

	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.ByteWidth = (sizeof(CameraUBO) + 255) & ~255;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	mDeviceRef->CreateBuffer(&cbDesc, nullptr, &mUniformBuffer);
}
