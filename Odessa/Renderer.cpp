#include "pch.h"
#include "Renderer.h"

Renderer::Renderer(GLFWwindow* window) : mWindow(window)
{
	mFactory = nullptr;

	mDevice = nullptr;
	mDeviceContext = nullptr;

	mSwapchain = nullptr;

	mRenderTarget = nullptr;
	mRTV = nullptr;

	mInputLayout = nullptr;

	mVertexShader = nullptr;
	mPixelShader = nullptr;
}

Renderer::~Renderer()
{
	mInputLayout->Release();

	mVertexShader->Release();
	mPixelShader->Release();

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

	mDeviceContext->OMSetRenderTargets(1, &mRTV, nullptr);

	const float clearColor[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRTV, clearColor);
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
}

void Renderer::InitPipeline()
{
	D3D11_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
	  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	  { "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	auto VSBytecode = Read("VertexShader.cso");
	auto PSBytecode = Read("PixelShader.cso");

	mDevice->CreateVertexShader(VSBytecode.data(), VSBytecode.size(), nullptr, &mVertexShader);
	mDevice->CreatePixelShader(PSBytecode.data(), PSBytecode.size(), nullptr, &mPixelShader);

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
