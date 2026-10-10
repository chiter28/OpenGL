#include "glTFLoader.h"

#include <iostream>
#include <utility>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <limits>
#include <vector>
#include <variant>
#include <cmath>

#include <glm/glm.hpp>

#include <fastgltf/core.hpp>
#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/tools.hpp>

#include <mikktspace.h>


namespace
{

	struct TangentGenerationData
	{
		const MeshData& Mesh;
		SubMesh Range;
		std::vector<glm::vec4> CornerTangents;
	};

	// вспомагательные функции
	TangentGenerationData& GetTangentData(const SMikkTSpaceContext* context)
	{
		return *static_cast<TangentGenerationData*>(context->m_pUserData);
	}

	const ModelVertex& GetTangentVertex(const TangentGenerationData& data, size_t face, size_t corner)
	{
		const size_t indexInIndexBuffer = data.Range.IndexOffset + face * 3 + corner;
		const uint32_t localIndexInVertexBuffer = data.Mesh.Indices[indexInIndexBuffer];
		const size_t globalIndexInVertexBuffer = static_cast<size_t>(data.Range.VertexOffset) + localIndexInVertexBuffer;

		return data.Mesh.Vertices[globalIndexInVertexBuffer];
	}

	// Returns number of Faces
	int MikkGetNumFaces(const SMikkTSpaceContext* context)
	{
		const TangentGenerationData& data = GetTangentData(context);
		return static_cast<int>(data.Range.IndexCount / 3);
	}

	// Returns number of vertices on face
	int MikkGetNumVerticesOfFace(const SMikkTSpaceContext*, const int)
	{
		return 3;
	}

	// Returns Position
	void MikkGetPosition(const SMikkTSpaceContext* context, float positionOut[], const int face, const int corner)
	{
		const TangentGenerationData& data = GetTangentData(context);
		
		const ModelVertex& vertex = GetTangentVertex(data, static_cast<size_t>(face), static_cast<size_t>(corner));

		positionOut[0] = vertex.position.x;
		positionOut[1] = vertex.position.y;
		positionOut[2] = vertex.position.z;
	}

	// Returns Normal
	void MikkGetNormal(const SMikkTSpaceContext* context, float normalOut[], const int face, const int corner)
	{
		const TangentGenerationData& data = GetTangentData(context);

		const ModelVertex& vertex = GetTangentVertex(data, static_cast<size_t>(face), static_cast<size_t>(corner));

		normalOut[0] = vertex.normal.x;
		normalOut[1] = vertex.normal.y;
		normalOut[2] = vertex.normal.z;
	}

	// Returns UV
	void MikkGetTexCoord(const SMikkTSpaceContext* context, float texCoordOut[], const int face, const int corner)
	{
		const TangentGenerationData& data = GetTangentData(context);

		const ModelVertex& vertex = GetTangentVertex(data, static_cast<size_t>(face), static_cast<size_t>(corner));

		texCoordOut[0] = vertex.texCoord.x;
		texCoordOut[1] = vertex.texCoord.y;
	}
	
	// Set Tangent of corner on face
	void MikkSetTSpaceBasic(const SMikkTSpaceContext* context, const float tangent[], const float sign, const int face, const int corner)
	{
		TangentGenerationData& data = GetTangentData(context);

		const size_t cornerIndex = 3 * static_cast<size_t>(face) + static_cast<size_t>(corner);

		data.CornerTangents[cornerIndex] = glm::vec4(tangent[0], tangent[1], tangent[2], sign);
	}

	
	

	SMikkTSpaceInterface CreateMikkInterface()
	{
		SMikkTSpaceInterface interface{};

		interface.m_getNumFaces = &MikkGetNumFaces;
		interface.m_getNumVerticesOfFace = &MikkGetNumVerticesOfFace;
		interface.m_getPosition = &MikkGetPosition;
		interface.m_getNormal = &MikkGetNormal;
		interface.m_getTexCoord = &MikkGetTexCoord;
		interface.m_setTSpaceBasic = &MikkSetTSpaceBasic;
		return interface;
	}

	std::vector<glm::vec4> GenerateCornerTangents(const MeshData& mesh, const SubMesh& range)
	{

		if (range.IndexCount == 0 || range.IndexCount % 3 != 0)
		{
			throw std::invalid_argument("Tangent generation requires a non-empty triangle list");
		}

		if (range.IndexCount > static_cast<uint32_t>(std::numeric_limits<int>::max()))
		{
			throw std::length_error("Too many indices for MikkTSpace");
		}

		if (range.IndexOffset > mesh.Indices.size() || range.IndexCount > mesh.Indices.size() - range.IndexOffset)
		{
			throw std::out_of_range("SubMesh index range is out of bounds");
		}


		if (range.VertexOffset > mesh.Vertices.size())
		{
			throw std::out_of_range("SubMesh vertex offset is out of bounds");
		}

		for (size_t i = 0; i < range.IndexCount; ++i)
		{
			const size_t indexInIndexBuffer = range.IndexOffset + i;
			const uint32_t localIndexInVertexBuffer = mesh.Indices[indexInIndexBuffer];
			const size_t globalIndexInVertexBuffer = static_cast<size_t>(range.VertexOffset) + localIndexInVertexBuffer;

			if (globalIndexInVertexBuffer >= mesh.Vertices.size())
			{
				throw std::out_of_range("SubMesh vertex index is out of bounds");
			}
			
			// Position validation
			const ModelVertex& vertex = mesh.Vertices[globalIndexInVertexBuffer];
			if (!std::isfinite(vertex.position.x) || !std::isfinite(vertex.position.y) || !std::isfinite(vertex.position.z))
			{
				throw std::invalid_argument("Vertex position must be finite");
			}

			// UV validation
			if (!std::isfinite(vertex.texCoord.x) || !std::isfinite(vertex.texCoord.y))
			{
				throw std::invalid_argument("Vertex texCoord must be finite");
			}

			// Normal validation
			if (!std::isfinite(vertex.normal.x) || !std::isfinite(vertex.normal.y) || !std::isfinite(vertex.normal.z))
			{
				throw std::invalid_argument("Vertex normal must be finite");
			}
			const float normalLength = glm::length(vertex.normal);
			if (!std::isfinite(normalLength) || normalLength <= 0.0f)
			{
				throw std::invalid_argument("Vertex normal must have a valid length");
			}
			if (std::abs((normalLength - 1)) > 0.0001f)
			{
				throw std::invalid_argument("Vertex normal must be normalized");
			}

		}



		// Generation
		TangentGenerationData data{ mesh, range, {} };
		data.CornerTangents.resize(range.IndexCount);

		SMikkTSpaceInterface interface = CreateMikkInterface();

		SMikkTSpaceContext context{};
		context.m_pInterface = &interface;
		context.m_pUserData = &data;

		if (!genTangSpaceDefault(&context)) // here
		{
			throw std::runtime_error("MikkTSpace tangent generation failed");
		}

		return std::move(data.CornerTangents);
	}





	glm::vec4 CalculateVertexTangent(const ModelVertex& v0, const ModelVertex& v1, const ModelVertex& v2)
	{
		// Получаем два движения по поверхности
		const glm::vec3 edge1 = v1.position - v0.position;
		const glm::vec3 edge2 = v2.position - v0.position;

		// Получаем соответствующие движения в UV
		const glm::vec2 dUV1 = v1.texCoord - v0.texCoord;
		const glm::vec2 dUV2 = v2.texCoord - v0.texCoord;

		// Находим оставшееся изменение U
		const float d = dUV1.x * dUV2.y - dUV2.x * dUV1.y;

		if (!std::isfinite(d) || std::abs(d) <= 0.00000001)
		{
			throw std::invalid_argument("Cannot calculate tangent from these UVs");
		}

		// Убираем изменение V
		const glm::vec3 directionU =  (edge1 * dUV2.y - edge2 * dUV1.y) / d;
		const glm::vec3 directionV = -(edge1 * dUV2.x - edge2 * dUV1.x) / d;
		



		// Получаем единичную нормаль именно вершины 0
		const float normalLength = glm::length(v0.normal);

		if (!std::isfinite(normalLength) || normalLength <= 0.0f)
		{
			throw std::invalid_argument("Vertex normal must be non-zero and finite");
		}

		const glm::vec3 N = v0.normal / normalLength;




		// Убираем составляющую вдоль N
		const glm::vec3 tangentInPlane = directionU - N * glm::dot(directionU, N);

		const float tangentLength = glm::length(tangentInPlane);

		if (!std::isfinite(tangentLength) || tangentLength <= 0.0f)
		{
			throw std::invalid_argument("Cannot normalize calculated tangent");
		}

		const glm::vec3 T = tangentInPlane / tangentLength;




		// Установим ориентацию B
		const glm::vec3 candidateB = glm::cross(N, T);
		const float orientation = glm::dot(candidateB, directionV);

		if (!std::isfinite(orientation) || orientation == 0.0f)
		{
			throw std::invalid_argument("Cannot determine tangent orientation");
		}
		const float sign = orientation < 0.0f ? -1.0f : 1.0f;

		return glm::vec4(T, sign);

	}







	void AppendPrimitive(
		const fastgltf::Asset& asset,
		const fastgltf::Primitive& primitive,
		MeshData& destination,
		uint32_t defaultMaterialIndex)
	{
		if (primitive.type != fastgltf::PrimitiveType::Triangles)
		{
			throw std::runtime_error("Only triangle primitives supported");
		}


		// какой accessor описывает эти данные (его индекс)
		const fastgltf::Attribute* positionAttribute = primitive.findAttribute("POSITION");
		const fastgltf::Attribute* normalAttribute = primitive.findAttribute("NORMAL");
		const fastgltf::Attribute* uvAttribute = primitive.findAttribute("TEXCOORD_0");
		const fastgltf::Attribute* tangentAttribute = primitive.findAttribute("TANGENT");

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

		// Tangents
		if (tangentAttribute != primitive.attributes.end())
		{
			const fastgltf::Accessor& tangents = asset.accessors.at(tangentAttribute->accessorIndex);

			if (tangents.type != fastgltf::AccessorType::Vec4)
			{
				throw std::runtime_error("TANGENT must contain vec4 elements");
			}

			if (tangents.count != positions.count)
			{
				throw std::runtime_error("Tangent and position counts do not match");
			}


			fastgltf::iterateAccessorWithIndex<glm::vec4>(asset, tangents,
				[&destination, vertexOffset](glm::vec4 tangent, size_t index)
				{
					destination.Vertices[index + vertexOffset].tangent = tangent;
				}
			);
		}


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
		uint32_t materialIndex = defaultMaterialIndex;
		if (primitive.materialIndex.has_value())
		{
			const size_t sourceIndex = primitive.materialIndex.value();
			if (sourceIndex >= asset.materials.size())
			{
				throw std::runtime_error("Primitive material index is out of range");
			}
			materialIndex = static_cast<uint32_t>(sourceIndex);
		}

		destination.SubMeshes.emplace_back(
			SubMesh
			{
				.IndexOffset = static_cast<uint32_t>(indexOffset),
				.IndexCount = static_cast<uint32_t>(indices.count),
				.VertexOffset = static_cast<uint32_t>(vertexOffset),
				.MaterialIndex = materialIndex
			}
		);

		if (tangentAttribute == primitive.attributes.end() && indices.count != 0)
		{
			if (indices.count > maxVertices - vertexOffset)
			{
				throw std::length_error("Generated geometry exceeds the vertex buffer size limit");
			}
			
			// tangents generation
			const std::vector<glm::vec4> cornerTangents = GenerateCornerTangents(destination, destination.SubMeshes.back());
			
			// vertices replacement
			std::vector<ModelVertex> cornerVertices;
			cornerVertices.reserve(cornerTangents.size());

			for (size_t i = 0; i < cornerTangents.size(); ++i)
			{
				const size_t localIndexInVertexBuffer = destination.Indices[indexOffset + i];
				ModelVertex vertex = destination.Vertices[vertexOffset + localIndexInVertexBuffer];
				vertex.tangent = cornerTangents[i];
				vertex.tangent.w = -vertex.tangent.w; // Flip w-channel (only for tangent generated by MikkTSpace for glTF)

				cornerVertices.push_back(vertex);
			}

			destination.Vertices.resize(vertexOffset);
			destination.Vertices.insert(destination.Vertices.end(), cornerVertices.begin(), cornerVertices.end());


			// indices replacement
			for (size_t i = 0; i < cornerTangents.size(); ++i)
			{
				destination.Indices[indexOffset + i] = static_cast<uint32_t>(i);
			}
		}
	}
	


	TextureWrap ReadTextureWrap(const fastgltf::Wrap source)
	{
		switch (source)
		{
			case fastgltf::Wrap::Repeat:
				return TextureWrap::Repeat;
			case fastgltf::Wrap::MirroredRepeat:
				return TextureWrap::MirroredRepeat;
			case fastgltf::Wrap::ClampToEdge:
				return TextureWrap::ClampToEdge;
		}

		throw std::runtime_error("Unsupported glTF texture wrap");
	}


	TextureSamplerDescription ReadSampler(const fastgltf::Sampler& source)
	{
		TextureSamplerDescription result{};

		// Mag Filter
		if (source.magFilter.has_value())
		{
			switch (source.magFilter.value())
			{
				case fastgltf::Filter::Nearest:
					result.MagFilter = TextureMagFilter::Nearest;
					break;
				case fastgltf::Filter::Linear:
					result.MagFilter = TextureMagFilter::Linear;
					break;

				default:
					throw std::runtime_error("Invalid glTF magnification filter");
			}
		}

		// Min Filter
		if (source.minFilter.has_value())
		{
			switch (source.minFilter.value())
			{
				case fastgltf::Filter::Nearest:
					result.MinFilter = TextureMinFilter::Nearest;
					break;
				case fastgltf::Filter::Linear:
					result.MinFilter = TextureMinFilter::Linear;
					break;
				case fastgltf::Filter::NearestMipMapNearest:
					result.MinFilter = TextureMinFilter::NearestMipMapNearest;
					break;
				case fastgltf::Filter::LinearMipMapNearest:
					result.MinFilter = TextureMinFilter::LinearMipMapNearest;
					break;
				case fastgltf::Filter::NearestMipMapLinear:
					result.MinFilter = TextureMinFilter::NearestMipMapLinear;
					break;
				case fastgltf::Filter::LinearMipMapLinear:
					result.MinFilter = TextureMinFilter::LinearMipMapLinear;
					break;

				default:
					throw std::runtime_error("Invalid glTF minification filter");
			}
		}

		result.WrapU = ReadTextureWrap(source.wrapS);
		result.WrapV = ReadTextureWrap(source.wrapT);

		return result;
	}


	uint32_t AppendFileTexture(
		const fastgltf::Asset& asset,
		const fastgltf::TextureInfo& info,
		const std::filesystem::path& directory,
		TextureColorSpace colorSpace,
		ModelData& destination)
	{
		if (info.texCoordIndex != 0 || info.transform)
		{
			throw std::runtime_error("This step supports only TEXCOORD_0 without texture transforms");
		}


		const fastgltf::Texture& texture = asset.textures.at(info.textureIndex);

		TextureSamplerDescription sampler{};
		if (texture.samplerIndex.has_value())
		{
			sampler = ReadSampler(asset.samplers.at(texture.samplerIndex.value()));
		}

		if (!texture.imageIndex.has_value())
		{
			throw std::runtime_error("Texture has no image source");
		}

		const fastgltf::Image& image = asset.images.at(texture.imageIndex.value());
		const fastgltf::sources::URI* source = std::get_if<fastgltf::sources::URI>(&image.data);

		if (!source || !source->uri.isLocalPath() || source->fileByteOffset != 0)
		{
			throw std::runtime_error("This step supports only external image files");
		}

		const std::filesystem::path imagePath = (directory / source->uri.fspath()).lexically_normal();

		if (!std::filesystem::is_regular_file(imagePath))
		{
			throw std::runtime_error("Image file does not exist: " + imagePath.string());
		}

		if (destination.Textures.size() >= MaterialData::InvalidTextureIndex)
		{
			throw std::runtime_error("Too many model textures");
		}

		const uint32_t textureIndex = static_cast<uint32_t>(destination.Textures.size());

		destination.Textures.emplace_back(
			TextureData
			{
				.Data = FileTextureData	{.Path = imagePath },
				.ColorSpace = colorSpace,
				.Sampler = sampler
			}
		);

		return textureIndex;
	}


	MaterialData ReadMaterial(const fastgltf::Material& source)
	{
		if (source.alphaMode != fastgltf::AlphaMode::Opaque)
		{
			throw std::runtime_error("Only opaque glTF materials are supported at this step");
		}

		const fastgltf::PBRData& pbr = source.pbrData;
	
		MaterialData result;

		result.BaseColor = glm::vec4(
			pbr.baseColorFactor[0],
			pbr.baseColorFactor[1],
			pbr.baseColorFactor[2],
			pbr.baseColorFactor[3]
		);

		result.Metallic = pbr.metallicFactor;
		result.Roughness = pbr.roughnessFactor;
		
		result.Emissive = glm::vec3(
			source.emissiveFactor[0],
			source.emissiveFactor[1],
			source.emissiveFactor[2]
		);
		

		return result;
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

		// Material
		if (asset.materials.size() >= std::numeric_limits<uint32_t>::max())
		{
			throw std::runtime_error("Too many glTF materials");
		}

		data.Materials.reserve(asset.materials.size() + 1);

		for (const fastgltf::Material& material : asset.materials)
		{

			MaterialData materialData = ReadMaterial(material);


			const fastgltf::PBRData& pbr = material.pbrData;


			if (pbr.baseColorTexture.has_value())
			{
				materialData.BaseColorTextureIndex = AppendFileTexture(
					asset,
					pbr.baseColorTexture.value(),
					path.parent_path(),
					TextureColorSpace::sRGB,
					data);
			}

			if (pbr.metallicRoughnessTexture.has_value())
			{
				materialData.MetallicRoughnessTextureIndex = AppendFileTexture(
					asset,
					pbr.metallicRoughnessTexture.value(),
					path.parent_path(),
					TextureColorSpace::Linear,
					data);
			}

			if (material.normalTexture.has_value())
			{
				const fastgltf::NormalTextureInfo& normalInfo = material.normalTexture.value();

				materialData.NormalTextureIndex = AppendFileTexture(
					asset,
					normalInfo,
					path.parent_path(),
					TextureColorSpace::Linear,
					data);

				materialData.NormalScale = normalInfo.scale;
			}

			data.Materials.emplace_back(materialData);
		}

		// Default Material
		const uint32_t defaultMaterialIndex = static_cast<uint32_t>(data.Materials.size());
		data.Materials.emplace_back(ReadMaterial(fastgltf::Material{}));


		// Mesh
		for (size_t gltfMeshIndex = 0; gltfMeshIndex < asset.meshes.size(); gltfMeshIndex++)
		{
			MeshData mesh;
			for (const fastgltf::Primitive& primitive : asset.meshes[gltfMeshIndex].primitives)
			{
				if (!primitive.targets.empty())
				{
					throw std::runtime_error("Morth targets are not supported yet");
				}

				AppendPrimitive(asset, primitive, mesh, defaultMaterialIndex);
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