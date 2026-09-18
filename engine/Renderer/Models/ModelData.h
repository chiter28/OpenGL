#pragma once

#include <vector>
#include <variant>
#include <filesystem>
#include <limits>

#include "Renderer/Resources/Layout.h"
#include "Renderer/Geometry/MeshTypes.h"


// ---------------------------
// Texture Data
// ---------------------------
//
struct FileTextureData
{
	std::filesystem::path Path;
};

struct EncodedTextureData
{
	std::vector<uint8_t> Bytes;
};

struct RawTextureData
{
	std::vector<uint8_t> Pixels;
	uint32_t Width = 0;
	uint32_t Height = 0;
};

using Source = std::variant<FileTextureData, EncodedTextureData, RawTextureData>;

struct TextureData
{
	Source Data;
};



// ---------------------------
// Material Data
// ---------------------------
//
struct MaterialData//
{
	static constexpr uint32_t InvalidTextureIndex = std::numeric_limits<uint32_t>::max();

	glm::vec4 BaseColor{ 1.0f };

	float Metallic = 0.0f;
	float Roughness = 1.0f;

	glm::vec3 Emissive{ 0.0f };

	// Temporary Blinn-Phong parameters while the current shader is used.
	glm::vec3 SpecularColor{ 1.0f };
	float Shininess = 400.0f;

	uint32_t BaseColorTextureIndex = InvalidTextureIndex;
	uint32_t NormalTextureIndex = InvalidTextureIndex;
	uint32_t MetallicRoughnessTextureIndex = InvalidTextureIndex;
	uint32_t OcclusionTextureIndex = InvalidTextureIndex;
	uint32_t EmissiveTextureIndex = InvalidTextureIndex;

	bool Transparent = false;

};



// ---------------------------
// Mesh Data
// ---------------------------
//
struct MeshData
{
	std::vector<ModelVertex> Vertices;
	std::vector<uint32_t> Indices;
	std::vector<SubMesh> SubMeshes;
};



// ===========================
// Model Data
// ===========================
// 
// Intermediate CPU-only representation produced by a model loader.
// It owns no Shader, Texture, VertexArray or other OpenGL resource.
struct ModelData
{
	std::vector<MeshData> Meshes;
	std::vector<MaterialData> Materials;
	std::vector<TextureData> Textures;
};

// Checks the CPU contract before uploading resources. Throws std::invalid_argument.
void ValidateModelData(const ModelData& data);
