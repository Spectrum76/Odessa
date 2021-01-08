#include "pch.h"
#include "Model.h"

Model::Model(Renderer* renderer) : Model(renderer->GetDevice(), renderer->GetContext())
{
}

Model::Model(ID3D11Device* device, ID3D11DeviceContext* context) : mDeviceRef(device), mDeviceContextRef(context)
{
}

Model::~Model()
{
}

void Model::Draw()
{
	for (auto Meshlet : MeshComponent)
	{
		Meshlet->Render();
	}
}

void Model::Update()
{
	D3D11_MAPPED_SUBRESOURCE cbRes;
	mDeviceContextRef->Map(mUniformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbRes);
	CopyMemory(cbRes.pData, &__Data, sizeof(Transform));
	mDeviceContextRef->Unmap(mUniformBuffer, 0);
}

void Model::CreateUBO()
{
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));

	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.ByteWidth = (sizeof(Transform) + 255) & ~255;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	mDeviceRef->CreateBuffer(&cbDesc, nullptr, &mUniformBuffer);
}
