#include "pch.h"
#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

Model::Model(Renderer* renderer, std::string filename) : Model(renderer->GetDevice(), renderer->GetContext(), filename)
{
}

Model::Model(ID3D11Device* device, ID3D11DeviceContext* context, std::string filename) : mDeviceRef(device), mDeviceContextRef(context)
{
	mUniformBuffer = nullptr;
	mFilename = filename;

	__Data.FModel = glm::mat4(1.0f);
	__Data.iModel = glm::mat4(1.0f);
}

Model::~Model()
{
	mUniformBuffer->Release();
}

void Model::Draw()
{
	for (auto Meshlet : MeshComponent)
	{
		Meshlet->Render();
	}
}

void Model::Load()
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	std::vector<Vertex> Vertices;
	std::vector<uint32_t> Indices;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, mFilename.c_str()))
	{
		spdlog::error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};

	for (const auto& shape : shapes)
	{
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
