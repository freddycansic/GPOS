#include "Scene.h"

#include <memory>
#include <optional>
#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Camera.h"
#include "engine/Debug.h"
#include "engine/input/Buttons.h"
#include "engine/input/Input.h"
#include "engine/rendering/ObjectRenderer.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/objects/Cube.h"
#include "engine/rendering/objects/Model.h"

std::vector<std::unique_ptr<Object>> s_Objects;
std::vector<Object*> s_SelectedObjects;
std::unique_ptr<Gizmo> sp_Gizmo;

Vec3 getSelectionCenter()
{
	Vec3 center;
	for (const auto& object : s_SelectedObjects)
	{
		center += object->getAvgPosition();
	}

	center /= static_cast<float>(s_SelectedObjects.size());

	return center;
}

std::optional<Object*> findClosestIntersectingObject(const Ray& ray, const Vec3& position)
{
	std::optional<Object*> closest;

	if (s_Objects.empty()) return closest; // no selectable object in empty scene

	float closestDistance = std::numeric_limits<float>::max();

	for (auto& object : s_Objects)
	{
		if (object->getAABB().isRayIntersecting(ray))
		{
			if (const auto& rayIntersection = object->isRayIntersecting(ray); rayIntersection.has_value())
			{
				const auto& pointOfIntersection = rayIntersection.value();

				const auto distanceFromPointToIntersection = static_cast<float>(std::pow(pointOfIntersection.x - position.x, 2)) + static_cast<float>(std::pow(pointOfIntersection.y - position.y, 2)) + static_cast<float>(std::pow(pointOfIntersection.z - position.z, 2));

				if (distanceFromPointToIntersection < closestDistance)
				{
					closestDistance = distanceFromPointToIntersection;
					closest.emplace(&*object); // dereference unique ptr, get regular pointer
				}
			}
		}
	}

	return closest;
}

namespace Scene
{
	void clearSelection()
	{
		for (const auto& selectedObject : s_SelectedObjects)
		{
			selectedObject->selected = false;
		}

		s_SelectedObjects.clear();
	}

	void selectObject(Object* obj)
	{
		s_SelectedObjects.push_back(obj);
		obj->selected = true;
	}

	void render()
	{
		for (const auto& object : s_Objects)
		{
			ObjectRenderer::draw(*object);
		}

		if (!s_SelectedObjects.empty())
		{
			sp_Gizmo->render(getSelectionCenter());
		}
	}

	void handleMouseClicks()
	{ // TODO make this readable
		static bool s_UsingGizmo = false;
		static Vec2 s_FirstMousePos;
		static std::optional<Vec3> s_IntersectingAxis;
		static Vec2 s_CentreToFirstMouseScreen, s_SelectionCentreScreen;

		// TODO add cancelling gizmo movements

		if (s_UsingGizmo && MouseButtons::MOUSE_1->isDown())
		{
			const auto mousePos = Input::getMousePos();

			const auto centreToCurrentMouse = mousePos - s_SelectionCentreScreen;

			const auto mag = s_CentreToFirstMouseScreen.dot(centreToCurrentMouse) / static_cast<float>(std::pow(s_CentreToFirstMouseScreen.magnitude(), 2));

			for (const auto& object : s_SelectedObjects)
			{
				sp_Gizmo->getOffsetTransformation(s_IntersectingAxis.value() * -1 * (mag - 1))(*object);
			}

			return;
		}

		if (s_UsingGizmo && MouseButtons::MOUSE_1->isJustReleased())
		{
			for (const auto& object : s_SelectedObjects)
			{
				object->applyOffset();
			}

			s_UsingGizmo = false;

			return;
		}

		if (!MouseButtons::MOUSE_1->isDown()) return;
		if (s_Objects.empty()) return; // cant select anything from empty scene

		const auto& mouseRay = Camera::perspRayFromCameraScreenPos(Input::getMousePos());

		if (!s_SelectedObjects.empty())
		{
			if (s_IntersectingAxis = sp_Gizmo->getIntersectingHandleAxis(mouseRay); s_IntersectingAxis.has_value())
			{
				if (!s_UsingGizmo)
				{
					s_FirstMousePos = Input::getMousePos();

					const auto& centre = getSelectionCenter();
					const Vec4 projectedCentre = Vec4(centre, 1.0f) * Renderer::getViewProjectionMatrix();
					const Vec2 flattenedCentreNDC = { projectedCentre.x / projectedCentre.w, projectedCentre.y / projectedCentre.w };

					s_SelectionCentreScreen = Camera::NDCToScreenBounds(flattenedCentreNDC);
					s_CentreToFirstMouseScreen = s_FirstMousePos - s_SelectionCentreScreen; // TODO only get once at first click
				}

				s_UsingGizmo = true;

				return;
			}
		}

		const auto closestObject = findClosestIntersectingObject(mouseRay, Camera::getPos());

		if (!Keys::LEFT_CONTROL->isDown())
		{
			clearSelection();
		}

		if (closestObject.has_value())
		{
			selectObject(closestObject.value());

			if (sp_Gizmo == nullptr) sp_Gizmo = std::make_unique<TranslateGizmo>();
		}
	}

	Assimp::Importer importer;

	void Scene::loadModel(const char* path)
	{
		// if the model has already been loaded
		if (Model::modelMeshes.contains(path))
		{
			const auto& models = Model::modelMeshes.at(path);

			for (size_t modelIdx = 0; modelIdx < models.size(); ++modelIdx)
			{
				addObject(std::make_unique<Model>(path, modelIdx, models.at(modelIdx).second));
			}

			return;
		}

		// load model and store in all models
		const auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		ASSERT_WITH_MSG(scene || !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode, importer.GetErrorString());

		for (size_t meshIdx = 0; meshIdx < scene->mNumMeshes; ++meshIdx)
		{
			const auto assimpMesh = scene->mMeshes[meshIdx];

			// get positions
			const auto p_MeshPositions = reinterpret_cast<Vec3*>(assimpMesh->mVertices);
			const std::vector<Vec3> meshPositions(p_MeshPositions, p_MeshPositions + assimpMesh->mNumVertices);

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
			//const auto p_MeshIndices = reinterpret_cast<GLuint*>(assimpMesh->mFaces[0].mIndices[0]);
			//std::vector<GLuint> meshIndices(p_MeshIndices, p_MeshIndices + assimpMesh->mNumFaces * 3);
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

			Model::modelMeshes[path].emplace_back(Mesh(meshPositions, meshTextureCoords, meshNormals, meshIndices), meshMaterial);

			addObject(std::make_unique<Model>(path, meshIdx, meshMaterial));

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

	void duplicateCurrentSelected()
	{
		for (const auto& object : s_SelectedObjects)
		{
			s_Objects.push_back(object->clone());

			// deselect old object
			s_Objects.at(s_Objects.size() - 1)->selected = false;
		}
	}

	void addObject(std::unique_ptr<Object>&& objectPtr)
	{
		s_Objects.push_back(std::move(objectPtr));
	}

	void setGizmoToTranslate()
	{
		sp_Gizmo = std::make_unique<TranslateGizmo>();
	}

	void setGizmoToScale()
	{
		sp_Gizmo = std::make_unique<ScaleGizmo>();
	}

	void setGizmoToRotate()
	{
		sp_Gizmo = std::make_unique<RotateGizmo>();
	}

	const std::vector<Object*>& getSelectedObjects()
	{
		return s_SelectedObjects;
	}
	const std::vector<std::unique_ptr<Object>>& getObjects()
	{
		return s_Objects;
	}
}