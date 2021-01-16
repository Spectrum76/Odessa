#pragma once
#include "pch.h"
#include "Renderer.h"

template <typename T>
class Uniform
{
public:
	Uniform(Renderer* renderer) : mDeviceRef(renderer->GetDevice()), mContextRef(renderer->GetContext())
	{
		D3D11_BUFFER_DESC cbDesc{};
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.ByteWidth = (sizeof(T) + 255) & ~255;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		mDeviceRef->CreateBuffer(&cbDesc, nullptr, &mUniformBuffer);
	}

	~Uniform()
	{
		mUniformBuffer->Release();
	}

	void Update(T* pData)
	{
		D3D11_MAPPED_SUBRESOURCE cbRes;
		pContextRef->Map(mUniformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbRes);
		memcpy(cbRes.pData, pData, sizeof(T));
		pContextRef->Unmap(mUniformBuffer, 0);
	}

	virtual void Bind(UINT slot) = 0;

protected:
	ID3D11Device* mDeviceRef;
	ID3D11DeviceContext* mContextRef;

	ID3D11Buffer* mUniformBuffer;
};

template<typename T>
class VSUniform : public Uniform<T>
{
public:
	void Bind(UINT slot) noexcept override
	{
		mContextRef->VSSetConstantBuffers(slot, 1, &mUniformBuffer);
	}
};

template<typename T>
class PSUniform : public Uniform<T>
{
public:
	void Bind(UINT slot) noexcept override
	{
		mContextRef->PSSetConstantBuffers(slot, 1, &mUniformBuffer);
	}
};

template<typename T>
class CSUniform : public Uniform<T>
{
public:
	void Bind(UINT slot) noexcept override
	{
		mContextRef->CSSetConstantBuffers(slot, 1, &mUniformBuffer);
	}
};
