#pragma once

class Model;
class Renderer;

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

