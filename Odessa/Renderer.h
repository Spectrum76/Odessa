#pragma once
class Renderer
{
public:
	Renderer(GLFWwindow* window);
	~Renderer();

	void Init();
	void Render();

protected:
	void InitAPI();
	void InitSwapchain();
	void InitFrameBuffer();

private:
	GLFWwindow* mWindow;

	IDXGIFactory4* mFactory;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;

	IDXGISwapChain1* mSwapchain;

	ID3D11Texture2D* mRenderTarget;
	ID3D11RenderTargetView* mRTV;
};

