#include "Scene.h"

#include <memory>
#include <optional>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include <stack>

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
#include "engine/Window.h"
#include "engine/rendering/gui/GUI.h"

using json = nlohmann::json;

struct SceneData
{
	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<Model>> selectedModels;
};

std::vector<std::shared_ptr<Model>> s_Models;
std::vector<std::shared_ptr<Model>> s_SelectedModels;

std::stack<SceneData> s_UndoScenes;
std::stack<SceneData> s_RedoScenes;

Vec3 s_BackgroundColour = { 0, 0, 0 };
std::vector<std::pair<std::string, size_t>> s_PreviousScenes;

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

void sortPreviousScenes(std::vector<std::pair<std::string, size_t>>& prevScenes)
{
	std::ranges::sort(prevScenes,
		[](const auto& scene1, const auto& scene2)
		{
			return scene1.second > scene2.second;
		});
}

void addToPreviousScenes(const std::string& path)
{
	using namespace std::chrono;

	const auto currentTime = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();

	bool found = false;
	for (auto& [prevPath, time] : s_PreviousScenes)
	{
		if (prevPath == path)
		{
			time = currentTime;
			found = true;
			break;
		}
	}

	if (!found) s_PreviousScenes.emplace_back(path, currentTime);

	sortPreviousScenes(s_PreviousScenes);
}

namespace Scene
{
	void init()
	{
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

	constexpr const char* PREVIOUS_SCENES_FILE = "previous_scenes.json";

	const std::vector<std::pair<std::string, size_t>>& getPreviousScenes()
	{
		if (static bool first = true; first)
		{
			std::ifstream previousScenesIn(PREVIOUS_SCENES_FILE);

			json previousScenes;
			// if the file has someting in it
			if (previousScenesIn) previousScenesIn >> previousScenes;

			previousScenesIn.close();

			for (const auto& [path, time] : previousScenes.items())
			{
				// if it still exists then show in launcher
				std::ifstream scene(path.c_str());
				if (scene.good())
				{
					std::cout << path << " still exists. Loading..." << std::endl;
					s_PreviousScenes.emplace_back(path, time);
				}

				scene.close();
			}

			sortPreviousScenes(s_PreviousScenes);

			first = false;
		}

		return s_PreviousScenes;
	}

	void savePreviousScenes()
	{
		json previousScenes;

		for (const auto& [path, time] : s_PreviousScenes)
		{
			// update times
			previousScenes[path] = time;
		}

		std::ofstream output(PREVIOUS_SCENES_FILE);
		output << previousScenes.dump();

		output.close();
	}

	void loadFromFile()
	{
		const auto& savePath = Files::getPathFromDialogue("gpos");

		if (!savePath) return;

		loadFromFile(savePath);
	}

	void loadFromFile(const char* path)
	{
		addToPreviousScenes(path);

		s_Models.clear();
		s_SelectedModels.clear();

		std::ifstream inputScene(path);

		Window::setTitle(Util::extractFileName(path));

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
			}
			else
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

		const auto& savePath = Files::getSavePathFromDialogue("gpos");
		if (!savePath) return;

		addToPreviousScenes(savePath);

		std::ofstream output(savePath);

		output << scene.dump();

		output.close();
	}

	constexpr size_t MAX_HISTORY = 10;
	void markNewAction()
	{
		// TODO FIX
		Util::clearStack(s_RedoScenes);

		if (s_UndoScenes.size() >= MAX_HISTORY)
		{
			Util::removeStackIndex(s_UndoScenes, 0);
		}

		s_UndoScenes.emplace(s_Models, s_SelectedModels);
	}

	void undo()
	{
		if (s_UndoScenes.empty()) return;

		// save current state if we want to go back
		s_RedoScenes.emplace(s_Models, s_SelectedModels);

		// take us back a step
		s_Models = s_UndoScenes.top().models;
		s_SelectedModels = s_UndoScenes.top().selectedModels;
		s_UndoScenes.pop();
	}
	
	void redo()
	{
		if (s_RedoScenes.empty()) return;

		s_UndoScenes.emplace(s_Models, s_SelectedModels);

		s_Models = s_RedoScenes.top().models;
		s_SelectedModels = s_RedoScenes.top().selectedModels;
		s_RedoScenes.pop();
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
		markNewAction();

		std::vector<std::shared_ptr<Model>> newModels;

		for (const auto& model : s_Models)
		{
			if (!model->selected)
			{
				newModels.push_back(model);
			}
		}

		s_SelectedModels.clear(); // dont call clearSelection() cause pointers will be invalid

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
	{
		static bool s_UsingGizmo = false;
		static Vec2 s_FirstMousePos;
		static std::optional<Vec3> s_IntersectingAxis;
		static Vec2 s_CentreToFirstMouseScreen, s_SelectionCentreScreen;

		if (GUI::isMouseHoveringAnyWindows()) return;

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
			if (!s_SelectedModels.empty()) 
			{
				markNewAction();
			}

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
		markNewAction();
		
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
			markNewAction();
			
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