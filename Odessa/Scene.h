#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Model.h"
#include "Renderer.h"

class Scene
{
public:
	Scene(Renderer* renderer);
	~Scene();

	void AddModel(std::string filename);
	void DrawScene();
	void DrawScene(ID3D11DeviceContext* DefCtx);

private:
	Renderer* mRendererRef;

	std::vector<Model*> mSceneModels;
};

