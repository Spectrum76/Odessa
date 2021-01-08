#pragma once
#include "Mesh.h"
#include "Types.h"
#include "Renderer.h"

class Model
{
public:
	Model(Renderer* renderer, std::string filename);
	Model(ID3D11Device* device, ID3D11DeviceContext* context, std::string filename);
	~Model();

	void Draw();

protected:
	void Load();
	void Update();
	void CreateUBO();

private:
	std::vector<Mesh*> MeshComponent;
	Transform __Data;
	std::string mFilename;

	ID3D11Buffer* mUniformBuffer;

	ID3D11Device* mDeviceRef;
	ID3D11DeviceContext* mDeviceContextRef;
};

