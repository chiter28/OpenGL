#pragma once

#include <filesystem>

#include "Renderer/Models/ModelData.h"

namespace glTFLoader
{
	ModelData LoadScene(const std::filesystem::path& path);
}