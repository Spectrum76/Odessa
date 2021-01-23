#pragma once

struct Transform;
class Texture;

class Model
{
public:
	Model(Renderer* renderer, std::string filename);
	~Model();

	void Draw();
	void Draw(ID3D11DeviceContext* DefCtx);

	void Position(glm::vec3 pos);
	void Rotation(glm::vec3 axis, float angle);
	void Scale(glm::vec3 scale);

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

