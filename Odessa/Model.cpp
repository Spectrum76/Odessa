#include "pch.h"
#include "Mesh.h"
#include "Types.h"
#include "Texture.h"
#include "Renderer.h"
#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

Model::Model(Renderer* renderer, std::string filename) : mDeviceRef(renderer->GetDevice()), mDeviceContextRef(renderer->GetContext())
{
	mUniformBuffer = nullptr;

	__Data.FModel = glm::mat4(1.0f);
	__Data.iModel = glm::mat4(1.0f);

	Load(filename, renderer);
	CreateUBO();
	Update();
}

Model::~Model()
{
	mUniformBuffer->Release();
}

void Model::Draw()
{
	mDeviceContextRef->VSSetConstantBuffers(0, 1, &mUniformBuffer);

	for (auto Meshlet : MeshComponent)
	{
		Meshlet.second->Bind(0);
		Meshlet.first->Render();
	}
}

void Model::DrawDeferred(ID3D11DeviceContext* DefCtx)
{
	DefCtx->VSSetConstantBuffers(0, 1, &mUniformBuffer);

	for (auto Meshlet : MeshComponent)
	{
		Meshlet.second->BindDeferred(DefCtx, 0);
		Meshlet.first->DrawDeferred(DefCtx);
	}
}

void Model::Position(glm::vec3 pos)
{
	__Data.FModel = glm::translate(__Data.FModel, pos);
	__Data.iModel = glm::inverse(__Data.FModel);
	Update();
}

void Model::Rotation(glm::vec3 axis, float angle)
{
	__Data.FModel = glm::rotate(__Data.FModel, angle, axis);
	__Data.iModel = glm::inverse(__Data.FModel);
	Update();
}

void Model::Scale(glm::vec3 scale)
{
	__Data.FModel = glm::scale(__Data.FModel, scale);
	__Data.iModel = glm::inverse(__Data.FModel);
	Update();
}

void Model::Load(std::string filename, Renderer* renderer)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str()))
	{
		spdlog::error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};

	for (const auto& shape : shapes)
	{
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;

		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex;

			vertex.Position =
			{
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.TexCoord =
			{
					   attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.Normal =
			{
				-(attrib.normals[3 * index.normal_index + 0]),
				-(attrib.normals[3 * index.normal_index + 1]),
				-(attrib.normals[3 * index.normal_index + 2])
			};

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(Vertices.size());
				Vertices.push_back(vertex);
			}

			Indices.push_back(uniqueVertices[vertex]);
		}

		Texture* texture = new Texture(renderer, materials[shape.mesh.material_ids[0]].diffuse_texname);

		Mesh* mesh = new Mesh(mDeviceRef, mDeviceContextRef);
		mesh->Initialize(Vertices, Indices);
		
		MeshComponent.push_back(std::make_pair(mesh, texture));
	}
}

void Model::Update()
{
	D3D11_MAPPED_SUBRESOURCE cbRes;
	mDeviceContextRef->Map(mUniformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbRes);
	CopyMemory(cbRes.pData, &__Data, sizeof(Transform));
	mDeviceContextRef->Unmap(mUniformBuffer, 0);
}

void Model::CreateUBO()
{
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));

	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.ByteWidth = (sizeof(Transform) + 255) & ~255;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	mDeviceRef->CreateBuffer(&cbDesc, nullptr, &mUniformBuffer);
}
