#include "Model.h"

#include <utility> // std::move

namespace
{
	std::shared_ptr<Texture> CreateTexture(const TextureData& textureData)
	{
		return std::visit(
			[](const auto& source) -> std::shared_ptr<Texture> // source - FileTextureData, EncodedTextureData, RawTextureData
			{
				// type? (FileTextureData, EncodedTextureData, RawTextureData)
				using SourceType = std::decay_t<decltype(source)>;

				if constexpr (std::is_same_v<SourceType, FileTextureData>)
				{
					// std::filesystem::path Path;
					if (!std::filesystem::exists(source.Path))
						return nullptr;
					return std::make_shared<Texture>(source.Path.string());
				}

				else if constexpr (std::is_same_v<SourceType, EncodedTextureData>)
				{
					// std::vector<uint8_t> Bytes;
					if (source.Bytes.empty())
						return nullptr;
					return std::make_shared<Texture>(source.Bytes.data(), static_cast<uint32_t>(source.Bytes.size()));
				}

				else if constexpr (std::is_same_v<SourceType, RawTextureData>)
				{
					// std::vector<uint8_t> Pixels;
					// uint32_t Width = 0;
					// uint32_t Height = 0;
					const uint32_t expectedSize = source.Width * source.Height * 4;
					if (source.Width == 0 || source.Height == 0 || source.Pixels.size() < expectedSize)
						return nullptr;
					return std::make_shared<Texture>(source.Pixels.data(), source.Width, source.Height);
				}

			}, textureData.Data);
	}
}

Model::Model(ModelData data)
{
	ValidateModelData(data);

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

		material->Emissive = materialData.Emissive;


		// Temporary Blinn-Phong parameters while the current shader is used.
		material->SpecularColor = materialData.SpecularColor;
		material->Shininess = materialData.Shininess;

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
