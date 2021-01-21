#pragma once
#include "RenderPass.h"
class GBufferPass : public RenderPass
{
public:
	GBufferPass(ID3D11Device* device, ID3D11DeviceContext* context);
	~GBufferPass();

	void Execute() override;
	void RecordExecution() override;

protected:
	void CreateFramebuffer();
	void CreatePipeline();

private:
	ID3D11Texture2D* gPosition;
	ID3D11RenderTargetView* gPosView;
	ID3D11ShaderResourceView* gPosSRV;

	ID3D11Texture2D* gNormal;
	ID3D11RenderTargetView* gNormView;
	ID3D11ShaderResourceView* gNormSRV;

	ID3D11Texture2D* gAlbedo;
	ID3D11RenderTargetView* gAlbedoView;
	ID3D11ShaderResourceView* gAlbedoSRV;

	ID3D11Texture2D* mDSBuffer;
	ID3D11DepthStencilView* mDSView;
	ID3D11DepthStencilState* mDSState;

	ID3D11InputLayout* mInputLayout;
	ID3D11RasterizerState* mRasterState;
	ID3D11SamplerState* mSamplerState;

	D3D11_VIEWPORT mViewport;

	ID3D11VertexShader* mGBufferVertexShader;
	ID3D11PixelShader* mGBufferPixelShader;
};

