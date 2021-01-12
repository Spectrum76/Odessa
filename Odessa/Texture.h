#pragma once
class Texture
{
public:
	Texture(Renderer* renderer, std::string filename);
	~Texture();

protected:
	void CreateTBO(std::string filename);

private:
	ID3D11Texture2D* mTexture;
	ID3D11ShaderResourceView* mSRV;

	ID3D11Device* mDeviceRef;
	ID3D11DeviceContext* mDeviceContextRef;
};

