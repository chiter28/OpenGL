#include "Model.h"

#include <utility> // std::move
#include <stdexcept>

#include "Renderer/Materials/Material.h"


namespace
{
	std::shared_ptr<Texture> CreateTexture(const TextureData& textureData)
	{
		return std::visit(
			[colorSpace = textureData.ColorSpace, sampler = textureData.Sampler](const auto& source) -> std::shared_ptr<Texture> // source - FileTextureData, EncodedTextureData, RawTextureData
			{
				// type? (FileTextureData, EncodedTextureData, RawTextureData)
				using SourceType = std::decay_t<decltype(source)>;

				
				// std::filesystem::path Path;
				if constexpr (std::is_same_v<SourceType, FileTextureData>)
				{
					return std::make_shared<Texture>(source.Path.string(), colorSpace, sampler);
				}


				// std::vector<std::byte> Bytes;
				else if constexpr (std::is_same_v<SourceType, EncodedTextureData>)
				{
					if (source.Bytes.empty())
					{
						throw std::invalid_argument("Encoded texture data must not be empty");
					}
					return std::make_shared<Texture>(source.Bytes.data(), source.Bytes.size(), colorSpace, sampler);
				}


				// std::vector<uint8_t> Pixels;
				// uint32_t Width = 0;
				// uint32_t Height = 0;
				else if constexpr (std::is_same_v<SourceType, RawTextureData>)
				{
					if (source.Width == 0 || source.Height == 0)
					{
						throw std::invalid_argument("Texture dimensions must be positive");
					}

					constexpr size_t bytesPerPixel = 4;
					const size_t availablePixels = source.Pixels.size() / bytesPerPixel;
					const size_t availableRows = availablePixels / source.Width;
					if (availableRows < source.Height)
					{
						throw std::invalid_argument("Texture pixel data is too small");
					}

					return std::make_shared<Texture>(source.Pixels.data(), source.Width, source.Height, colorSpace, sampler);
				}

			}, textureData.Data);
	}
}



Model::Model(ModelData data)
{
	ValidateModelData(data);

	m_Instances = std::move(data.Instances);

	// move Mesh Data to m_Meshes
	m_Meshes.reserve(data.Meshes.size());

	for (auto& meshData : data.Meshes)
	{
		m_Meshes.emplace_back(
	        std::move(meshData.Vertices),
			std::move(meshData.Indices),
			std::move(meshData.SubMeshes)
		);
	}




	// Textures
	std::vector<std::shared_ptr<Texture>> textures;
	textures.reserve(data.Textures.size());

	for (const auto& textureData : data.Textures)
	{
		textures.emplace_back(CreateTexture(textureData));
	}

	auto resolveTexture = [&textures](uint32_t index) -> std::shared_ptr<Texture>
		{
			if (index == MaterialData::InvalidTextureIndex)
				return nullptr;
			if (index >= textures.size())
				throw std::out_of_range("Invalid texture index");

			return textures[index];
		};

	
	// move Material Data to m_Materials
	m_Materials.reserve(data.Materials.size());
	for (const auto& materialData : data.Materials)
	{
		auto material = std::make_shared<Material>();
		material->BaseColor = materialData.BaseColor;
		
		material->Metallic = materialData.Metallic;
		material->Roughness = materialData.Roughness;

		material->NormalScale = materialData.NormalScale;

		material->Emissive = materialData.Emissive;


		material->BaseColorTexture = resolveTexture(materialData.BaseColorTextureIndex);
		material->NormalTexture = resolveTexture(materialData.NormalTextureIndex);;
		material->MetallicRoughnessTexture = resolveTexture(materialData.MetallicRoughnessTextureIndex);;
		material->OcclusionTexture = resolveTexture(materialData.OcclusionTextureIndex);;
		material->EmissiveTexture = resolveTexture(materialData.EmissiveTextureIndex);;

		material->Transparent = materialData.Transparent;

		m_Materials.emplace_back(std::move(material));
	}
}









void Model::AddMesh(Mesh mesh)
{
	m_Meshes.emplace_back(std::move(mesh));
}


void Model::AddMaterial(std::shared_ptr<Material> material)
{
	m_Materials.emplace_back(std::move(material));
}
