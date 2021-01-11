#pragma once
#include "Mesh.h"
#include "Types.h"
#include "Renderer.h"

class Model
{
public:
	Model(Renderer* renderer, std::string filename);
	~Model();

	void Draw();

protected:
	void Load(std::string filename);
	void Update();
	void CreateUBO();

private:
	std::vector<Mesh*> MeshComponent;
	Transform __Data;

	ID3D11Buffer* mUniformBuffer;

	ID3D11Device* mDeviceRef;
	ID3D11DeviceContext* mDeviceContextRef;
};

