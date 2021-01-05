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
}

Renderer::~Renderer()
{
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
}

void Renderer::Render()
{
	const float clearColor[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRTV, clearColor);
	mSwapchain->Present(0, 0);
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
