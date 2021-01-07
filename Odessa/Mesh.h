#pragma once
#include "Types.h"
#include "Renderer.h"

class Mesh
{
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* context);
	Mesh(Renderer* renderer);
	~Mesh();

	void Render();
	void Initialize(std::vector<Vertex> Vertices, std::vector<uint32_t> Indices);

protected:
	void CreateVBO(std::vector<Vertex> Vertices);
	void CreateIBO(std::vector<uint32_t> Indices);

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;

	UINT IndexCount;

	UINT Stride;
	UINT Offset;

	ID3D11Device* mDeviceRef;
	ID3D11DeviceContext* mDeviceContextRef;
};

