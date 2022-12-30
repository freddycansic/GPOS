#include "Model.h"

#include "engine/Debug.h"

std::unordered_map<const char*, std::vector<std::pair<Mesh, Material>>> Model::modelMeshes;

Model::Model(const char* path, size_t index, const Material& material)
	: Object(material), m_Path(path), m_Index(index)
{
}

Mesh& Model::getMesh() const
{
	return modelMeshes.at(m_Path)[m_Index].first;
}

[[nodiscard]] std::unique_ptr<Object> Model::clone() const
{
	return std::make_unique<Model>(*this);
}