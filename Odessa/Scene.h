#pragma once

class Model;
class Renderer;
class Camera;

class Scene
{
public:
	Scene(Renderer* renderer, Camera* camera);
	~Scene();

	void AddModel(std::string filename);
	void DrawScene();
	void DrawScene(ID3D11DeviceContext* DefCtx);

private:
	Renderer* mRendererRef;
	Camera* mCameraRef;

	std::vector<Model*> mSceneModels;
};

