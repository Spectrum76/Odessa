#include "pch.h"
#include "GBufferPass.h"
#include "Utilities.h"

GBufferPass::GBufferPass(ID3D11Device* device, ID3D11DeviceContext* context) : RenderPass(device, context)
{
	gPosition = nullptr;
	gPosView = nullptr;
	gNormal = nullptr;
	gNormView = nullptr;
	gAlbedo = nullptr;
	gAlbedoView = nullptr;

	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = static_cast<float>(WIDTH);
	mViewport.Height = static_cast<float>(HEIGHT);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	mInputLayout = nullptr;
	mRasterState = nullptr;
	mSamplerState = nullptr;

	gPosSRV = nullptr;
	gNormSRV = nullptr;
	gAlbedoSRV = nullptr;
	mDSBuffer = nullptr;
	mDSView = nullptr;
	mDSState = nullptr;
	mGBufferVertexShader = nullptr;
	mGBufferPixelShader = nullptr;
}

GBufferPass::~GBufferPass()
{
	gPosition->Release();
	gPosView->Release();
	gNormal->Release();
	gNormView->Release();
	gAlbedo->Release();
	gAlbedoView->Release();

	mInputLayout->Release();
	mRasterState->Release();
	mSamplerState->Release();

	gPosSRV->Release();
	gNormSRV->Release();
	gAlbedoSRV->Release();
	mDSBuffer->Release();
	mDSView->Release();
	mDSState->Release();
	mGBufferVertexShader->Release();
	mGBufferPixelShader->Release();
}

void GBufferPass::Execute()
{
	mImmCtxRef->ExecuteCommandList(mCmdList, false);
}

void GBufferPass::RecordExecution()
{
	mDefCtx->VSSetShader(mGBufferVertexShader, 0, 0);
	mDefCtx->PSSetShader(mGBufferPixelShader, 0, 0);
	mDefCtx->PSSetSamplers(0, 1, &mSamplerState);

	mDefCtx->IASetInputLayout(mInputLayout);
	mDefCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mDefCtx->RSSetViewports(1, &mViewport);
	mDefCtx->RSSetState(mRasterState);

	ID3D11RenderTargetView* mRTVs[] = { gAlbedoView, gNormView, gPosView };

	mDefCtx->OMSetRenderTargets(1, mRTVs, mDSView);
	mDefCtx->OMSetDepthStencilState(mDSState, 1);

	const float clearColor[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	
	for (size_t i = 0; i < _countof(mRTVs); i++)
	{
		mDefCtx->ClearRenderTargetView(mRTVs[i], clearColor);
	}
	
	mDefCtx->ClearDepthStencilView(mDSView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void GBufferPass::CreateFramebuffer()
{
	D3D11_TEXTURE2D_DESC GDesc{};
	GDesc.Width = WIDTH;
	GDesc.Height = HEIGHT;
	GDesc.MipLevels = 1;
	GDesc.ArraySize = 1;
	GDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	GDesc.SampleDesc.Count = 1;
	GDesc.SampleDesc.Quality = 0;
	GDesc.Usage = D3D11_USAGE_DEFAULT;
	GDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = GDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	mDeviceRef->CreateTexture2D(&GDesc, nullptr, &gAlbedo);
	mDeviceRef->CreateTexture2D(&GDesc, nullptr, &gNormal);
	mDeviceRef->CreateTexture2D(&GDesc, nullptr, &gPosition);

	mDeviceRef->CreateRenderTargetView(gNormal, nullptr, &gNormView);
	mDeviceRef->CreateRenderTargetView(gPosition, nullptr, &gPosView);
	mDeviceRef->CreateRenderTargetView(gAlbedo, nullptr, &gAlbedoView);

	mDeviceRef->CreateShaderResourceView(gNormal, nullptr, &gNormSRV);
	mDeviceRef->CreateShaderResourceView(gPosition, nullptr, &gPosSRV);
	mDeviceRef->CreateShaderResourceView(gAlbedo, nullptr, &gAlbedoSRV);

	D3D11_TEXTURE2D_DESC dSDesc{};
	dSDesc.Width = WIDTH;
	dSDesc.Height = HEIGHT;
	dSDesc.MipLevels = 1;
	dSDesc.ArraySize = 1;
	dSDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dSDesc.SampleDesc.Count = 1;
	dSDesc.SampleDesc.Quality = 0;
	dSDesc.Usage = D3D11_USAGE_DEFAULT;
	dSDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	mDeviceRef->CreateTexture2D(&dSDesc, nullptr, &mDSBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	mDeviceRef->CreateDepthStencilView(mDSBuffer, &descDSV, &mDSView);

	D3D11_DEPTH_STENCIL_DESC dssDesc{};
	dssDesc.DepthEnable = TRUE;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS;

	mDeviceRef->CreateDepthStencilState(&dssDesc, &mDSState);
}

void GBufferPass::CreatePipeline()
{
	D3D11_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
	  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	  { "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_RASTERIZER_DESC rasterDesc{};
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_SOLID;

	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	auto VSBytecode = Read("GBufferVertexShader.cso");
	auto PSBytecode = Read("GBufferPixelShader.cso");

	mDeviceRef->CreateVertexShader(VSBytecode.data(), VSBytecode.size(), nullptr, &mGBufferVertexShader);
	mDeviceRef->CreatePixelShader(PSBytecode.data(), PSBytecode.size(), nullptr, &mGBufferPixelShader);

	mDeviceRef->CreateSamplerState(&samplerDesc, &mSamplerState);
	mDeviceRef->CreateRasterizerState(&rasterDesc, &mRasterState);
	mDeviceRef->CreateInputLayout(inputElementDescs, _countof(inputElementDescs), VSBytecode.data(), VSBytecode.size(), &mInputLayout);
}
