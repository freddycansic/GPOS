#include "Scene.h"

#include <memory>
#include <optional>
#include <algorithm>
#include <unordered_map>
#include <fstream>

#include <json/json.hpp>

#include "Camera.h"
#include "engine/Debug.h"
#include "engine/input/Buttons.h"
#include "engine/input/Files.h"
#include "engine/input/Input.h"
#include "engine/rendering/ObjectRenderer.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/objects/Model.h"
#include "engine/rendering/objects/Cube.h"
#include "engine/rendering/objects/Line.h"
#include "engine/Colours.h"

std::vector<std::shared_ptr<Model>> s_Models;
std::vector<std::shared_ptr<Model>> s_SelectedModels;
Vec3 s_BackgroundColour = { 0, 0, 0 };

Vec3 getSelectionCenter()
{
	Vec3 center = {0, 0, 0};
	for (const auto& model : s_SelectedModels)
	{
		center += model->getAvgPosition();
	}

	center /= static_cast<float>(s_SelectedModels.size());
	return center;
}

std::optional<std::shared_ptr<Model>> findClosestIntersectingModel(const Ray& ray, const Vec3& position)
{
	std::optional<std::shared_ptr<Model>> closest;

	if (s_Models.empty()) return closest; // no selectable model in empty scene

	float closestDistance = std::numeric_limits<float>::max();

	for (auto& model : s_Models)
	{
		if (model->getAABB().isRayIntersecting(ray))
		{
			if (const auto& rayIntersection = model->isRayIntersecting(ray); rayIntersection.has_value())
			{
				const auto& pointOfIntersection = rayIntersection.value();

				const auto distanceFromPointToIntersection = static_cast<float>(std::pow(pointOfIntersection.x - position.x, 2)) + static_cast<float>(std::pow(pointOfIntersection.y - position.y, 2)) + static_cast<float>(std::pow(pointOfIntersection.z - position.z, 2));

				if (distanceFromPointToIntersection < closestDistance)
				{
					closestDistance = distanceFromPointToIntersection;
					closest.emplace(model); // dereference unique ptr, get regular pointer
				}
			}
		}
	}

	return closest;
}

std::vector<Line> gridLines;
constexpr float GRIDLINE_LENGTH = 200.0f;
constexpr float AXES_LINE_WIDTH = 0.01f;

void drawAxes()
{
	using namespace Flags;

	static Line x(-GRIDLINE_LENGTH / 2, 0, 0, GRIDLINE_LENGTH / 2, 0, 0, AXES_LINE_WIDTH, Colours::RED);
	static Line z(0, 0, -GRIDLINE_LENGTH / 2, 0, 0, GRIDLINE_LENGTH / 2, AXES_LINE_WIDTH, Colours::BLUE);
	static Cube centreCube(0, 0, 0, 0.05f, Colours::WHITE);

	ObjectRenderer::draw(centreCube, NO_LIGHTING | ALWAYS_SOLID);
	ObjectRenderer::draw(x, NO_LIGHTING | ALWAYS_SOLID);
	ObjectRenderer::draw(z, NO_LIGHTING | ALWAYS_SOLID);

	for (auto& gridLine : gridLines)
	{
		ObjectRenderer::draw(gridLine, NO_LIGHTING | ALWAYS_SOLID);
	}
}

namespace Scene
{
	void init()
	{
		//s_Models.reserve();

		static constexpr float GRIDLINE_STEP = 2.5f;
		static constexpr float GRIDLINE_WIDTH = 0.005f;
		for (float i = -GRIDLINE_LENGTH / 2; i < GRIDLINE_LENGTH / 2; i += GRIDLINE_STEP)
		{
			gridLines.emplace_back
			(
				-GRIDLINE_LENGTH / 2, 0, i, 
				GRIDLINE_LENGTH / 2, 0, i,
				GRIDLINE_WIDTH, Colours::DEFAULT
			);

			gridLines.emplace_back
			(
				i, 0, -GRIDLINE_LENGTH / 2,
				i, 0, GRIDLINE_LENGTH / 2,
				GRIDLINE_WIDTH, Colours::DEFAULT
			);
		}
	}

	std::unordered_map<std::string, Texture> atlas;
	const Texture& getTexture(const std::string& path)
	{
		if (atlas.contains(path)) return atlas.at(path);

		atlas[path] = Texture(path);

		return atlas.at(path);
	}

	void loadFromFile()
	{
		const auto& savePath = Files::getPathFromDialogue("json");

		if (!savePath) return;

		s_Models.clear();
		s_SelectedModels.clear();

		std::ifstream inputScene(savePath);

		json scene;
		inputScene >> scene;

		inputScene.close();

		const auto& bgCol = scene["background_colour"];
		s_BackgroundColour = Vec3(bgCol[0], bgCol[1], bgCol[2]);

		for (const auto& jsonModel : scene["models"])
		{
			const auto& jsonTranslation = jsonModel["transform"]["translation"];
			const auto& jsonRotation = jsonModel["transform"]["rotation"];
			const auto& jsonScale = jsonModel["transform"]["scale"];

			const auto& translation = Vec3(jsonTranslation[0], jsonTranslation[1], jsonTranslation[2]);
			const auto& rotation = Vec3(jsonRotation[0], jsonRotation[1], jsonRotation[2]);
			const auto& scale = Vec3(jsonScale[0], jsonScale[1], jsonScale[2]);

			const auto& matCol = jsonModel["material"]["colour"];
			const auto& colour = Vec4(matCol[0], matCol[1], matCol[2], matCol[3]);

			// remove \" at the start and end of strings;
			const auto& rawModelPath = jsonModel["path"].dump();
			const auto modelPath = rawModelPath.substr(1, rawModelPath.size() - 2);

			const auto& rawTexturePath = jsonModel["material"]["texturePath"].dump();
			const auto texturePath = rawTexturePath.substr(1, rawTexturePath.size() - 2);
			
			Material mat;
			if (texturePath == "none")
			{
				mat = Material(colour);
			} else
			{
				mat = Material(texturePath, colour);
			}

			Model model(
				modelPath,
				jsonModel["index"],
				translation,
				1.0f,
				mat
			);

			model.setRotation(rotation);
			model.setScale(scale);

			s_Models.push_back(std::make_shared<Model>(model));
		}
	}

	void saveToFile()
	{
		json scene;

		// background colour
		scene["background_colour"] = std::vector{s_BackgroundColour.x, s_BackgroundColour.y, s_BackgroundColour.z};

		scene["models"];

		// models
		for (const auto& model : s_Models)
		{
			json jsonModel;
			jsonModel["path"] = model->getPath();
			jsonModel["index"] = model->getIndex();

			const auto& [tra, rot, sca] = model->getTransform();
			jsonModel["transform"]["translation"] = std::vector{ tra.x, tra.y, tra.z };
			jsonModel["transform"]["rotation"] = std::vector{ rot.x, rot.y, rot.z };
			jsonModel["transform"]["scale"] = std::vector{ sca.x, sca.y, sca.z };

			const auto& [colour, texPath] = model->material;
			jsonModel["material"]["colour"] = std::vector{ colour.x, colour.y, colour.z, colour.w };
			jsonModel["material"]["texturePath"] = texPath.empty() ? "none" : texPath;

			scene["models"].push_back(jsonModel);
		}

		const auto& savePath = Files::getSavePathFromDialogue("json");
		if (!savePath) return;

		std::ofstream output(savePath);

		output << scene.dump();

		output.close();
	}

	void clearSelection()
	{
		for (const auto& selectedModel : s_SelectedModels)
		{
			selectedModel->selected = false;
		}

		s_SelectedModels.clear();
	}

	void selectAll()
	{
		for (auto& model : s_Models)
		{
			selectModel(model);
		}
	}

	void selectModel(const std::shared_ptr<Model>& model)
	{
		if (model->selected) return;

		s_SelectedModels.push_back(model);
		model->selected = true;
	}

	void deleteSelected()
	{
		std::vector<std::shared_ptr<Model>> newModels;

		for (const auto& model : s_Models)
		{
			if (!model->selected)
			{
				newModels.push_back(model);
			}
		}

		s_SelectedModels.clear(); // dont call clearSelection cause pointers will be invalid

		s_Models = std::move(newModels);
	}

	void invertSelection()
	{
		s_SelectedModels.clear();

		for (auto& model : s_Models)
		{
			if (model->selected)
			{
				model->selected = false;
			} else
			{
				selectModel(model);
			}
		}
	}

	void render()
	{
		drawAxes();

		for (auto& model : s_Models)
		{
			ObjectRenderer::draw(*model);
		}

		if (!s_SelectedModels.empty())
		{
			Gizmo::render(getSelectionCenter());
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

			for (const auto& model : s_SelectedModels)
			{
				Gizmo::transformObject(*model, s_IntersectingAxis.value() * -1 * (mag - 1));
			}

			return;
		}

		if (s_UsingGizmo && MouseButtons::MOUSE_1->isJustReleased())
		{
			for (const auto& model : s_SelectedModels)
			{
				model->applyOffset();
			}

			s_UsingGizmo = false;

			return;
		}

		if (!MouseButtons::MOUSE_1->isDown()) return;
		if (s_Models.empty()) return; // cant select anything from empty scene

		const auto& mouseRay = Camera::perspRayFromCameraScreenPos(Input::getMousePos());

		if (!s_SelectedModels.empty())
		{
			if (s_IntersectingAxis = Gizmo::getIntersectingHandleAxis(mouseRay); s_IntersectingAxis.has_value())
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

		const auto closestModel = findClosestIntersectingModel(mouseRay, Camera::getPos());

		if (!Keys::LEFT_CONTROL->isDown())
		{
			clearSelection();
		}

		if (closestModel.has_value())
		{
			selectModel(closestModel.value());
		}
	}

	void duplicateCurrentSelected()
	{
		for (const auto& model : s_SelectedModels)
		{
			addModel(std::move(*model));

			// deselect new model
			s_Models.at(s_Models.size() - 1)->selected = false;
		}
	}

	void loadModelIntoScene()
	{
		if (const auto path = Files::getPathFromDialogue("amf,3ds,ac,ase,assbin,b3d,bvh,collada,dxf,csm,hmp,irrmesh,iqm,irr,lwo,lws,m3d,md2,md3,md5,mdc,mdl,nff,ndo,off,obj,ogre,opengex,ply,ms3d,cob,blend,ifc,xgl,fbx,q3d"); path)
		{
			Model::loadModelMeshes(path);
			
			const auto& models = Model::meshes.at(path);
				
			for (size_t modelIdx = 0; modelIdx < models.size(); ++modelIdx)
			{
				addModel(Model(path, modelIdx, 0, 0, 0, 1.0f, models.at(modelIdx).second));
			}
		}
	}

	void addModel(Model&& model)
	{
		s_Models.push_back(std::make_shared<Model>(std::move(model)));
	}

	std::vector<std::shared_ptr<Model>>& getSelectedModels()
	{
		return s_SelectedModels;
	}

	std::vector<std::shared_ptr<Model>>& getModels()
	{
		return s_Models;
	}

	Vec3& getMutRefBackgroundColour()
	{
		return s_BackgroundColour;
	}

	void setBackgroundColour(const Vec3& colour)
	{
		s_BackgroundColour = colour;
	}
}