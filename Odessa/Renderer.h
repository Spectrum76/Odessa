#pragma once

class Scene;
class GBufferPass;

class Renderer
{
public:
	Renderer(GLFWwindow* window);
	~Renderer();

	void Init();
	void Render();
	void Present();

	void Render(Scene* scene);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();

protected:
	void InitAPI();
	void InitSwapchain();
	void InitFrameBuffer();
	void InitPipeline();
	void InitPasses();

private:
	GLFWwindow* mWindow;

	IDXGIFactory4* mFactory;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;

	IDXGISwapChain1* mSwapchain;
	D3D11_VIEWPORT mViewport;

	ID3D11Texture2D* mRenderTarget;
	ID3D11RenderTargetView* mRTV;

	ID3D11Texture2D* mDSBuffer;
	ID3D11DepthStencilView* mDSView;
	ID3D11DepthStencilState* mDSState;

	GBufferPass* mGBufferPass;

	ID3D11InputLayout* mInputLayout;
	ID3D11RasterizerState* mRasterState;
	ID3D11SamplerState* mSamplerState;

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
};

