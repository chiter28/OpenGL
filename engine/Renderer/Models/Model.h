#pragma once 

#include <memory>
#include <vector>

#include "Renderer/Geometry/Mesh.h"

#include "ModelData.h"


class Material;


class Model
{
public:
	Model() = default;
	~Model() = default;
	
	// Move
	Model(Model&&) noexcept = default;
	Model& operator=(Model&&) noexcept = default;

	// Copy
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;



	explicit Model(ModelData data);


	// Add
	void AddMesh(Mesh mesh);
	void AddMaterial(std::shared_ptr<Material> material);

	// Get
	const std::vector<Mesh>& GetMeshes() const noexcept { return m_Meshes; }
	const std::vector<std::shared_ptr<Material>>& GetMaterials() const noexcept { return m_Materials; }
	
	// Get index
	const Mesh& GetMesh (size_t index) const { return m_Meshes.at(index); }
	const std::shared_ptr<Material>& GetMaterial(size_t index) const { return m_Materials.at(index); }

private:
	std::vector<Mesh> m_Meshes;
	std::vector<std::shared_ptr<Material>> m_Materials;
};