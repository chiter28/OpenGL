#include "ModelData.h"

#include <stdexcept>
#include <string>

void ValidateModelData(const ModelData& data)
{

	// Проверка MeshData на валидность
	for (size_t meshIndex = 0; meshIndex < data.Meshes.size(); ++meshIndex)
	{
		const auto& mesh = data.Meshes[meshIndex];
		for (size_t subMeshIndex = 0; subMeshIndex < mesh.SubMeshes.size(); ++subMeshIndex)
		{
			const auto& subMesh = mesh.SubMeshes[subMeshIndex];
			const auto prefix = "Mesh " + std::to_string(meshIndex)	+ ", SubMesh " + std::to_string(subMeshIndex) + ": ";

			// Subtraction avoids overflowing IndexOffset + IndexCount.
			// Индексов не больше чем нужно?
			if (subMesh.IndexOffset > mesh.Indices.size() || subMesh.IndexCount > mesh.Indices.size() - subMesh.IndexOffset)
				throw std::invalid_argument(prefix + "index range exceeds the index buffer");

			// Индексы делятся на 3?
			if (subMesh.IndexCount % 3 != 0)
				throw std::invalid_argument(prefix + "triangle index count must be divisible by three");

			// Индексов и Вершинных смещение не больше чем 32-bit значение?
			if (subMesh.IndexCount > static_cast<uint32_t>(std::numeric_limits<int32_t>::max())
				|| subMesh.VertexOffset > static_cast<uint32_t>(std::numeric_limits<int32_t>::max()))
					throw std::invalid_argument(prefix + "draw parameters exceed signed 32-bit limits");

			// The current renderer requires a material for every draw range.
			// Индекс материала не больше чем самых Метериалов
			if (subMesh.MaterialIndex >= data.Materials.size())
				throw std::invalid_argument(prefix + "material index is invalid");

			for (size_t i = 0; i < subMesh.IndexCount; ++i)
			{
				const auto index = mesh.Indices[static_cast<size_t>(subMesh.IndexOffset) + i];
				// Indices are local to the base vertex, not necessarily to the whole mesh.
				if (subMesh.VertexOffset >= mesh.Vertices.size() || index >= mesh.Vertices.size() - subMesh.VertexOffset)
					throw std::invalid_argument(prefix + "index plus base vertex exceeds the vertex buffer");
			}
		}
	}

	// Проверка инлекса тестур на InvalidTextureIndex + не привешает ли индекс текстуры само количество текстур
	for (size_t materialIndex = 0; materialIndex < data.Materials.size(); ++materialIndex)
	{
		const auto& material = data.Materials[materialIndex];
		
		const auto checkTexture = [&](uint32_t index, const char* slot)
		{
			if (index != MaterialData::InvalidTextureIndex && index >= data.Textures.size())
				throw std::invalid_argument("Material " + std::to_string(materialIndex)
					+ ": " + slot + " texture index is invalid");
		};

		checkTexture(material.BaseColorTextureIndex, "base color");
		checkTexture(material.NormalTextureIndex, "normal");
		checkTexture(material.MetallicRoughnessTextureIndex, "metallic-roughness");
		checkTexture(material.OcclusionTextureIndex, "occlusion");
		checkTexture(material.EmissiveTextureIndex, "emissive");
	}
}
