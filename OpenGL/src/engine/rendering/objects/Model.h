#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

#include "Object.h"

struct Model : Object
{
public:
	Model(const char* path, size_t index, const Material& material);
	Model() = default;
	~Model() override = default;

	[[nodiscard]] std::unique_ptr<Object> clone() const override;

	[[nodiscard]] Mesh& getMesh() const override;

	static std::unordered_map<const char*, std::vector<std::pair<Mesh, Material>>> modelMeshes;

private:
	const char* m_Path = nullptr;
	size_t m_Index = 0;
};