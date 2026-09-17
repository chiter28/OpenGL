#pragma once 

#include <memory>
#include <vector>

#include "Mesh2.h"
#include "Material2.h"



struct ModelData
{
	struct MeshData
	{
		std::vector<Mesh2::VertexType> Vertices;
		std::vector<uint32_t> Indices;
		std::vector<SubMesh2> SubMeshes;
	};

	std::vector<MeshData> Meshes;
	std::vector<std::shared_ptr<Material>> Materials;
};


class Model
{
public:
	Model() = default;
	~Model() = default;

	Model(Model&&) noexcept = default;
	Model& operator=(Model&&) noexcept = default;

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;



	explicit Model(ModelData data);


	void AddMesh(Mesh2 mesh);
	void AddMaterial(std::shared_ptr<Material> material);


	const std::vector<Mesh2>&                    GetMeshes()    const noexcept { return m_Meshes;    }
	const std::vector<std::shared_ptr<Material>> GetMaterials() const noexcept { return m_Materials; }

	const Mesh2&                      GetMesh    (size_t index) const { return m_Meshes.at(index);    }
	const std::shared_ptr<Material>& GetMaterial(size_t index) const { return m_Materials.at(index); }

private:
	std::vector<Mesh2> m_Meshes;
	std::vector<std::shared_ptr<Material>> m_Materials;
};