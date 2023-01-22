#include "Model.h"

#include <imgui/imgui.h>

#include "engine/Debug.h"
#include "engine/Util.h"

std::unordered_map<std::string, std::vector<std::pair<Mesh, Material>>> Model::meshes;
Assimp::Importer Model::s_Importer;

Model::Model(const std::string& path, size_t index, Vec3 pos, float scale, const Material& material)
	: Object(material), m_Path(path), m_Index(index)
{
	m_Name = Util::extractFileName(path) + std::string("_") + std::to_string(index);
	m_Name = Util::replaceAll(m_Name, " ", "_");

	m_Transform.tra = pos;
	m_Transform.sca = { scale, scale, scale };

	loadModelMeshes(path);
}

Model::Model(const std::string& path, size_t index, float x, float y, float z, float scale, const Material& material)
	: Model(path, index, Vec3(x, y, z), scale, material)
{}

void Model::loadModelMeshes(const std::string& path)
{
	// if the model has already been loaded
	if (!meshes.empty())
	{
		if (meshes.contains(path))
		{
			return;
		}
	}

	// load model and store in all models
	auto scene = s_Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		scene = s_Importer.ReadFile("res/models/model_not_found.obj", aiProcess_Triangulate | aiProcess_FlipUVs);

		ASSERT_WITH_MSG(scene || !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode, "Default model misplaced from res/models/model_not_found.obj!");
	}

	for (size_t meshIdx = 0; meshIdx < scene->mNumMeshes; ++meshIdx)
	{
		const auto assimpMesh = scene->mMeshes[meshIdx];

		// get positions
		const auto p_MeshPositions = reinterpret_cast<Vec3*>(assimpMesh->mVertices);
		std::vector<Vec3> meshPositions(p_MeshPositions, p_MeshPositions + assimpMesh->mNumVertices);

		// get normals
		const auto p_MeshNormals = reinterpret_cast<Vec3*>(assimpMesh->mNormals);
		const std::vector<Vec3> meshNormals(p_MeshNormals, p_MeshNormals + assimpMesh->mNumVertices);

		// get texture coordinates
		std::vector<Vec2> meshTextureCoords;
		meshTextureCoords.reserve(assimpMesh->mNumVertices);

		if (assimpMesh->mTextureCoords[0])
		{
			for (size_t texCoordIdx = 0; texCoordIdx < assimpMesh->mNumVertices; ++texCoordIdx)
			{
				meshTextureCoords.emplace_back(assimpMesh->mTextureCoords[0][texCoordIdx].x, assimpMesh->mTextureCoords[0][texCoordIdx].y);
			}
		}

		// get indices
		std::vector<GLuint> meshIndices;
		meshIndices.reserve(assimpMesh->mNumFaces * assimpMesh->mFaces[0].mNumIndices);

		for (size_t faceIdx = 0; faceIdx < assimpMesh->mNumFaces; ++faceIdx)
		{
			const auto& face = assimpMesh->mFaces[faceIdx];

			for (size_t indexIdx = 0; indexIdx < face.mNumIndices; ++indexIdx)
			{
				meshIndices.push_back(face.mIndices[indexIdx]);
			}
		}

		// get material
		const auto assimpMaterial = scene->mMaterials[assimpMesh->mMaterialIndex];
		aiColor3D colour;
		assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, colour);

		Material meshMaterial(Vec3(colour.r, colour.g, colour.b));

		meshes[path].emplace_back(Mesh(meshPositions, meshTextureCoords, meshNormals, meshIndices), meshMaterial);

		// TODO textures
		//std::vector<Texture> assimpMaterialTextures(assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE));
		//for (size_t textureIdx = 0; textureIdx < assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE); textureIdx++)
		//{
		//	aiString assmipTexturePath;
		//	assimpMaterial->GetTexture(aiTextureType_DIFFUSE, textureIdx, &assmipTexturePath);
		//	assimpMaterialTextures.emplace_back(assmipTexturePath.C_Str());
		//}
	}
}

Mesh& Model::getMesh() const
{
	return meshes.at(m_Path)[m_Index].first;
}

const std::string& Model::stringName() const
{
	return m_Name;
}

const std::string& Model::getPath() const
{
	return m_Path;
}

size_t Model::getIndex() const
{
	return m_Index;
}
