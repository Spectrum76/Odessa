#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* context) : mDeviceRef(device), mDeviceContextRef(context)
{
	mVertexBuffer = nullptr;
	mIndexBuffer = nullptr;

	IndexCount = 0;
	Stride = sizeof(Vertex);
	Offset = 0;
}

Mesh::~Mesh()
{
	mVertexBuffer->Release();
	mIndexBuffer->Release();
}

void Mesh::Initialize(std::vector<Vertex> Vertices, std::vector<uint32_t> Indices)
{
	CreateVBO(Vertices);
	CreateIBO(Indices);
}

void Mesh::CreateVBO(std::vector<Vertex> Vertices)
{
	D3D11_BUFFER_DESC VertexBufferDesc;
	ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));

	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(Vertex) * (UINT)Vertices.size();
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = &Vertices.front();
	vInitData.SysMemPitch = 0;
	vInitData.SysMemSlicePitch = 0;

	mDeviceRef->CreateBuffer(&VertexBufferDesc, &vInitData, &mVertexBuffer);
}

void Mesh::CreateIBO(std::vector<uint32_t> Indices)
{
	D3D11_BUFFER_DESC IndexBufferDesc;
	ZeroMemory(&IndexBufferDesc, sizeof(IndexBufferDesc));

	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned int) * (UINT)Indices.size();
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA iInitData;
	iInitData.pSysMem = &Indices.front();
	iInitData.SysMemPitch = 0;
	iInitData.SysMemSlicePitch = 0;

	IndexCount = (UINT)Indices.size();

	mDeviceRef->CreateBuffer(&IndexBufferDesc, &iInitData, &mIndexBuffer);
}

void Mesh::Render()
{
	mDeviceContextRef->IASetVertexBuffers(0, 1, &mVertexBuffer, &Stride, &Offset);
	mDeviceContextRef->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	mDeviceContextRef->DrawIndexed(IndexCount, 0, 0);
}
