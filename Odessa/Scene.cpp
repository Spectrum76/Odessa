#include "pch.h"
#include "Scene.h"

Scene::Scene(Renderer* renderer) : mRendererRef(renderer)
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
	for (auto model : mSceneModels)
	{
		model->Draw();
	}
}
