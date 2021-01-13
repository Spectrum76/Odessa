#pragma once
class Model
{
public:
	Model(Renderer* renderer, std::string filename);
	~Model();

	void Draw();

protected:
	void Load(std::string filename, Renderer* renderer);
	void Update();
	void CreateUBO();

private:
	std::vector<std::pair<Mesh*, Texture*>> MeshComponent;
	Transform __Data;

	ID3D11Buffer* mUniformBuffer;

	ID3D11Device* mDeviceRef;
	ID3D11DeviceContext* mDeviceContextRef;
};

