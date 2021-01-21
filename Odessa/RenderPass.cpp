#include "pch.h"
#include "RenderPass.h"

RenderPass::RenderPass(ID3D11Device* device, ID3D11DeviceContext* context) : mDeviceRef(device), mImmCtxRef(context)
{
	mCmdList = nullptr;
	mDefCtx = nullptr;

	mDeviceRef->CreateDeferredContext(0, &mDefCtx);
}

RenderPass::~RenderPass()
{
	mCmdList->Release();
	mDefCtx->Release();
}
