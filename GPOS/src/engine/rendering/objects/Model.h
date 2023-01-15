#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

#include <json/json.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Object.h"

using json = nlohmann::json;

struct Model final : Object
{
	Model(const std::string& path, size_t index, Vec3 pos, float scale, const Material& material);
	Model(const std::string& path, size_t index, float x, float y, float z, float scale, const Material& material);
	Model() = default;
	~Model() override = default;

	[[nodiscard]] Mesh& getMesh() const override;
	[[nodiscard]] const std::string& stringName() const override;

	[[nodiscard]] const std::string& getPath() const;
	[[nodiscard]] size_t getIndex() const;

	static void loadModelMeshes(const std::string& path);
	static std::unordered_map<std::string, std::vector<std::pair<Mesh, Material>>> meshes;

private:
	static Assimp::Importer s_Importer;

	std::string m_Path;
	std::string m_Name;
	size_t m_Index = 0;
};