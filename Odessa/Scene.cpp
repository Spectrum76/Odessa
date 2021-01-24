#include "pch.h"
#include "Mesh.h"
#include "Types.h"
#include "Texture.h"
#include "Model.h"
#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"

Scene::Scene(Renderer* renderer, Camera* camera) : mRendererRef(renderer), mCameraRef(camera)
{
}

Scene::~Scene()
{
	for (auto model : mSceneModels)
	{
		delete model;
	}
}

void Scene::AddModel(std::string filename)
{
	Model* model = new Model(mRendererRef, filename);
	mSceneModels.push_back(model);
}

void Scene::DrawScene()
{
	mCameraRef->Bind();

	for (auto model : mSceneModels)
	{
		model->Draw();
	}
}

void Scene::DrawScene(ID3D11DeviceContext* DefCtx)
{
	mCameraRef->Bind(DefCtx);

	for (auto model : mSceneModels)
	{
		model->Draw(DefCtx);
	}
}
