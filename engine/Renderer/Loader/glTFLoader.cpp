#include "glTFLoader.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <limits>
#include <vector>


#include <glm/glm.hpp>

#include <fastgltf/core.hpp>
#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/tools.hpp>


namespace
{

	void AppendPrimitive(const fastgltf::Asset& asset, const fastgltf::Primitive& primitive, MeshData& destination)
	{

		

		if (primitive.type != fastgltf::PrimitiveType::Triangles)
		{
			throw std::runtime_error("Only triangle primitives supported");
		}


		// какой accessor описывает эти данные (его индекс)
		const fastgltf::Attribute* positionAttribute = primitive.findAttribute("POSITION");
		const fastgltf::Attribute* normalAttribute = primitive.findAttribute("NORMAL");
		const fastgltf::Attribute* uvAttribute = primitive.findAttribute("TEXCOORD_0");

		if (positionAttribute == primitive.attributes.end() ||
			normalAttribute == primitive.attributes.end() ||
			uvAttribute == primitive.attributes.end())
		{
			throw std::runtime_error("This loading step requires POSITION, NORMAL, TEXCOORD_0");
		}


		// проверка наличия индексов
		if (!primitive.indicesAccessor.has_value())
		{
			throw std::runtime_error("This loading step requires an indexed primitive");
		}


		// получить описание элементов и расположения данных в бинарнике
		const fastgltf::Accessor& positions = asset.accessors.at(positionAttribute->accessorIndex);
		const fastgltf::Accessor& normals = asset.accessors.at(normalAttribute->accessorIndex);
		const fastgltf::Accessor& uvs = asset.accessors.at(uvAttribute->accessorIndex);
		const fastgltf::Accessor& indices = asset.accessors.at(primitive.indicesAccessor.value());

		// тип каждого элемента соотведствует типу в движке?
		if (positions.type != fastgltf::AccessorType::Vec3 ||
			normals.type != fastgltf::AccessorType::Vec3 ||
			uvs.type != fastgltf::AccessorType::Vec2 ||
			indices.type != fastgltf::AccessorType::Scalar)
		{
			throw std::runtime_error("Unexpected accessor shape");
		}

		// для каждой вершины существует нормаль и uv?
		if (normals.count != positions.count || uvs.count != positions.count)
		{
			throw std::runtime_error("Vertex attribute counts do not match");
		}

		// проверка количества индексов
		if (indices.count % 3 != 0 || indices.count > static_cast<size_t>(std::numeric_limits<int32_t>::max()))
		{
			throw std::runtime_error("Invalid triangle index count");
		}


		

		const size_t vertexOffset = destination.Vertices.size();
		const size_t indexOffset = destination.Indices.size();



		// Вычеслим сколько вершин можно передать в VertexBuffer при его создании
		// и сколько индексов — в IndexBuffer (VertexBuffer(const void* data, uint32_t size);).
		// Так как в конструкторе у них стоит - uint32_t size,
		// то максимум можно передать - maxBufferBytes байтов
		// 
		constexpr size_t maxBufferBytes = std::numeric_limits<uint32_t>::max();
		// максимальное количество вершин, байтовый размер которых можно передать через параметр uint32_t size в вершином конструкторе(in bytes).
		constexpr size_t maxVertices = maxBufferBytes / sizeof(ModelVertex);
		constexpr size_t maxIndices = maxBufferBytes / sizeof(uint32_t);
		
		// вершин уже больше чем можно записать при создании VertexBuffer в конструктор или
		// кол-во добавляемых вершин (positions.count) больше оставшегося места и превысят байтовый размер uint32_t
		if (vertexOffset > maxVertices || positions.count > maxVertices - vertexOffset)
		{
			throw std::runtime_error("Combined vertex buffer is to large");
		}

		if (indexOffset > maxIndices || indices.count > maxIndices - indexOffset)
		{
			throw std::runtime_error("Combined index buffer is to large");
		}





		destination.Vertices.resize(vertexOffset + positions.count);
		destination.Indices.reserve(indexOffset + indices.count);

		// vertices
		fastgltf::iterateAccessorWithIndex<glm::vec3>(asset, positions,
			[&destination, vertexOffset](glm::vec3 posVec3, size_t index)
			{
				destination.Vertices[index + vertexOffset].position = posVec3;
				destination.Vertices[index + vertexOffset].color = glm::vec3(1.0f);
			}
		);

		fastgltf::iterateAccessorWithIndex<glm::vec3>(asset, normals,
			[&destination, vertexOffset](glm::vec3 normVec3, size_t index)
			{
				destination.Vertices[index + vertexOffset].normal = normVec3;
			}
		);

		fastgltf::iterateAccessorWithIndex<glm::vec2>(asset, uvs,
			[&destination, vertexOffset](glm::vec2 uvVec2, size_t index)
			{
				destination.Vertices[index + vertexOffset].texCoord = uvVec2;
			}
		);

		// indices
		fastgltf::iterateAccessor<uint32_t>(asset, indices,
			[&destination, vertexCount = positions.count](uint32_t vertexIndex)
			{
				if (vertexIndex >= vertexCount)
				{
					throw std::runtime_error("Vertex index is out of range");
				}
				destination.Indices.push_back(vertexIndex);
			}
		);



		// subMesh
		destination.SubMeshes.emplace_back(
			SubMesh
			{
				.IndexOffset = static_cast<uint32_t>(indexOffset),
				.IndexCount = static_cast<uint32_t>(indices.count),
				.VertexOffset = static_cast<uint32_t>(vertexOffset),
				.MaterialIndex = 0
			}
		);
	}



	glm::mat4 Mat4ToGLM(const fastgltf::math::fmat4x4& source)
	{
		glm::mat4 result = {1.0f};
		
		for (int column = 0; column < 4; column++)
		{
			for (int row = 0; row < 4; row++)
			{
				result[column][row] = source[column][row];
			}
		}
		return result;
	}

}



namespace glTFLoader
{
	ModelData LoadScene(const std::filesystem::path& path)
	{
		// прочитать даные из файла и записать себе в file
		fastgltf::Expected<fastgltf::GltfDataBuffer> file = fastgltf::GltfDataBuffer::FromPath(path);

		if (file.error() != fastgltf::Error::None)
		{
			throw std::runtime_error("Cannot read glTF: " + std::string(fastgltf::getErrorMessage(file.error())));
		}


		// разобрать формат glTF
		fastgltf::Parser parser;

		constexpr auto options = fastgltf::Options::LoadExternalBuffers | fastgltf::Options::LoadGLBBuffers;

		fastgltf::Expected<fastgltf::Asset> parsed = parser.loadGltf(file.get(), path.parent_path(), options);

		if (parsed.error() != fastgltf::Error::None)
		{
			throw std::runtime_error("Cannot parse glTF: " + std::string(fastgltf::getErrorMessage(parsed.error())));
		}

		const fastgltf::Asset& asset = parsed.get();

		// проверить разобранные данные на соответствие требованиям glTF
		fastgltf::Error validationError = fastgltf::validate(asset);

		if (validationError != fastgltf::Error::None)
		{
			throw std::runtime_error("Invalid glTF: " + std::string(fastgltf::getErrorMessage(validationError)));
		}



		// получить индекс сцены
		if (asset.scenes.empty())
		{
			throw std::runtime_error("glTF containes no Scenes");
		}

		const size_t sceneIndex = asset.defaultScene.value_or(0);

		if (sceneIndex >= asset.scenes.size())
		{
			throw std::runtime_error("Invalid glTF scene index");
		}



		ModelData data;
		data.Materials.emplace_back(MaterialData{});


		for (size_t gltfMeshIndex = 0; gltfMeshIndex < asset.meshes.size(); gltfMeshIndex++)
		{
			MeshData mesh;
			for (const fastgltf::Primitive& primitive : asset.meshes[gltfMeshIndex].primitives)
			{
				if (!primitive.targets.empty())
				{
					throw std::runtime_error("Morth targets are not supported yet");
				}

				AppendPrimitive(asset, primitive, mesh);
			}

			data.Meshes.emplace_back(std::move(mesh));
		}

		// Обход узлов выбранной сцены и накопление матриц
		fastgltf::iterateSceneNodes(asset, sceneIndex, fastgltf::math::fmat4x4{ 1.0f },
			[&](const fastgltf::Node& node, const fastgltf::math::fmat4x4& nodeTransform)
			{
				if (node.skinIndex.has_value())
				{
					throw std::runtime_error("Skinned nodes are not supported yet");
				}

				if (!node.meshIndex.has_value())
				{
					return;
				}

				const glm::mat4 transform = Mat4ToGLM(nodeTransform);

				for (int column = 0; column < 4; column++)
				{
					for (int row = 0; row < 4; row++)
					{
						if (!std::isfinite(transform[column][row]))
						{
							throw std::runtime_error("Node transform contains a non-finite value");
						}
					}
				}

				const float determinant = glm::determinant(glm::mat3(transform));
				if (!std::isfinite(determinant) || determinant <= 0.0f)
				{
					throw std::runtime_error("Singular or mirrored node transforms are not supported yet");
				}

				data.Instances.emplace_back(
					MeshInstance
					{
						.MeshIndex = node.meshIndex.value(),
						.Transform = transform
					}
				);
			}
		);


		ValidateModelData(data);
		return data;
	}
}