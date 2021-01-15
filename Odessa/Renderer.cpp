#include "pch.h"
#include "Renderer.h"

Renderer::Renderer(GLFWwindow* window) : mWindow(window)
{
	mFactory = nullptr;

	mDevice = nullptr;
	mDeviceContext = nullptr;

	mSwapchain = nullptr;

	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = static_cast<float>(WIDTH);
	mViewport.Height = static_cast<float>(HEIGHT);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	mRenderTarget = nullptr;
	mRTV = nullptr;

	mDSBuffer = nullptr;
	mDSState = nullptr;
	mDSView = nullptr;

	mInputLayout = nullptr;
	mRasterState = nullptr;
	mSamplerState = nullptr;

	mVertexShader = nullptr;
	mPixelShader = nullptr;
}

Renderer::~Renderer()
{
	mInputLayout->Release();
	mRasterState->Release();
	mSamplerState->Release();

	mVertexShader->Release();
	mPixelShader->Release();

	mDSBuffer->Release();
	mDSState->Release();
	mDSView->Release();

	mRTV->Release();
	mRenderTarget->Release();

	mSwapchain->Release();

	mDeviceContext->Release();
	mDevice->Release();

	mFactory->Release();
}

void Renderer::Init()
{
	InitAPI();
	InitSwapchain();
	InitFrameBuffer();
	InitPipeline();
}

void Renderer::Render()
{
	mDeviceContext->VSSetShader(mVertexShader, 0, 0);
	mDeviceContext->PSSetShader(mPixelShader, 0, 0);
	mDeviceContext->PSSetSamplers(0, 1, &mSamplerState);

	mDeviceContext->IASetInputLayout(mInputLayout);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mDeviceContext->RSSetViewports(1, &mViewport);
	mDeviceContext->RSSetState(mRasterState);

	mDeviceContext->OMSetRenderTargets(1, &mRTV, mDSView);
	mDeviceContext->OMSetDepthStencilState(mDSState, 1);

	const float clearColor[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRTV, clearColor);
	mDeviceContext->ClearDepthStencilView(mDSView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::Present()
{
	mSwapchain->Present(0, 0);
}

ID3D11Device* Renderer::GetDevice()
{
	return mDevice;
}

ID3D11DeviceContext* Renderer::GetContext()
{
	return mDeviceContext;
}

void Renderer::InitAPI()
{
	CreateDXGIFactory2(0, IID_PPV_ARGS(&mFactory));

	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		NULL, D3D11_CREATE_DEVICE_DEBUG,
		NULL, NULL, D3D11_SDK_VERSION,
		&mDevice, nullptr, &mDeviceContext);
}

void Renderer::InitSwapchain()
{
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.BufferCount = ImageCount;
	swapchainDesc.Width = WIDTH;
	swapchainDesc.Height = HEIGHT;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.SampleDesc.Count = 1;

	HWND hwnd = glfwGetWin32Window(mWindow);

	mFactory->CreateSwapChainForHwnd(mDevice, hwnd, &swapchainDesc, nullptr, nullptr, &mSwapchain);
}

void Renderer::InitFrameBuffer()
{
	mSwapchain->GetBuffer(0, IID_PPV_ARGS(&mRenderTarget));

	mDevice->CreateRenderTargetView(mRenderTarget, nullptr, &mRTV);

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

	mDevice->CreateTexture2D(&dSDesc, nullptr, &mDSBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	mDevice->CreateDepthStencilView(mDSBuffer, &descDSV, &mDSView);

	D3D11_DEPTH_STENCIL_DESC dssDesc{};
	dssDesc.DepthEnable = TRUE;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS;

	mDevice->CreateDepthStencilState(&dssDesc, &mDSState);
}

void Renderer::InitPipeline()
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

	auto VSBytecode = Read("VertexShader.cso");
	auto PSBytecode = Read("PixelShader.cso");

	mDevice->CreateVertexShader(VSBytecode.data(), VSBytecode.size(), nullptr, &mVertexShader);
	mDevice->CreatePixelShader(PSBytecode.data(), PSBytecode.size(), nullptr, &mPixelShader);

	mDevice->CreateSamplerState(&samplerDesc, &mSamplerState);
	mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
	mDevice->CreateInputLayout(inputElementDescs, _countof(inputElementDescs), VSBytecode.data(), VSBytecode.size(), &mInputLayout);
}

std::vector<char> Renderer::Read(std::string File)
{
	std::ifstream read(File.c_str(), std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = read.tellg();

	std::vector<char> buff(pos);
	read.seekg(0, std::ios::beg);
	read.read(buff.data(), pos);

	return buff;
}
