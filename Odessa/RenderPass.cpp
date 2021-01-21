#include "pch.h"
#include "RenderPass.h"

RenderPass::RenderPass(ID3D11Device* device, ID3D11DeviceContext* context) : mDeviceRef(device), mDeviceContextRef(context)
{
	mCmdList = nullptr;
	mContext = nullptr;

	mDeviceRef->CreateDeferredContext(0, &mContext);
}

RenderPass::~RenderPass()
{
	mCmdList->Release();
	mContext->Release();
}
