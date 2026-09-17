#include "Model.h"

#include <utility> // std::move


Model::Model(ModelData data)
{
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

	// move Material Data to m_Materials
	m_Materials = std::move(data.Materials);
}

void Model::AddMesh(Mesh2 mesh)
{
	m_Meshes.emplace_back(std::move(mesh));
}


void Model::AddMaterial(std::shared_ptr<Material> material)
{
	m_Materials.emplace_back(std::move(material));
}
