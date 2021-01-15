#include "pch.h"
#include "Renderer.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(Renderer* renderer, std::string filename) :
	mDeviceRef(renderer->GetDevice()), mDeviceContextRef(renderer->GetContext())
{
	mTexture = nullptr;
	mSRV = nullptr;

	CreateTBO(filename);
}

Texture::~Texture()
{
	mSRV->Release();
	mTexture->Release();
}

void Texture::Bind(UINT slot)
{
	if (mSRV)
	{
		mDeviceContextRef->PSSetShaderResources(slot, 1, &mSRV);
	}
	else
	{
		ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
		mDeviceContextRef->PSSetShaderResources(slot, 1, nullSRV);
	}
}

void Texture::CreateTBO(std::string filename)
{
	if (filename.empty())
	{
		return;
	}

	int TextureWidth;
	int TextureHeight;
	int TextureChannel;
	unsigned char* Pixels = stbi_load(filename.c_str(), &TextureWidth, &TextureHeight, &TextureChannel, STBI_rgb_alpha);

	if (!Pixels)
	{
		spdlog::error("STB_IMAGE Failed!");
	}

	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = TextureWidth;
	desc.Height = TextureHeight;
	desc.MipLevels = 0;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	mDeviceRef->CreateTexture2D(&desc, nullptr, &mTexture);
	mDeviceContextRef->UpdateSubresource(mTexture, 0, nullptr, (const void*)Pixels, TextureWidth * 4, 0);

	mDeviceRef->CreateShaderResourceView(mTexture, &srvDesc, &mSRV);
	mDeviceContextRef->GenerateMips(mSRV);
}
