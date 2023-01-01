#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Object.h"

struct Model final : Object
{
public:
	Model(Vec3 pos, const char* path, size_t index, const Material& material);
	Model(float x, float y, float z, const char* path, size_t index, const Material& material);
	Model() = default;
	~Model() override = default;

	[[nodiscard]] std::unique_ptr<Object> clone() const override;
	[[nodiscard]] std::string stringName() const override;

	[[nodiscard]] Mesh& getMesh() const override;

	static void loadModelMeshes(const char* path);
	static std::unordered_map<const char*, std::vector<std::pair<Mesh, Material>>> meshes;

private:
	static Assimp::Importer s_Importer;

	const char* m_Path = nullptr;
	std::string m_Name;
	size_t m_Index = 0;
};