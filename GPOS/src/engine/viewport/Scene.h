#pragma once

#include <memory>
#include <concepts>

#include "engine/rendering/gui/Gizmo.h"
#include "engine/rendering/objects/Model.h"
#include "engine/rendering/opengl/Texture.h"

struct Ray;
struct Vec3;

namespace Scene
{
	void init();

	void addModel(Model&& model);

	void duplicateCurrentSelected();
	void loadModelIntoScene();

	void saveToFile();
	void loadFromFile();

	void selectModel(const std::shared_ptr<Model>& model);
	void clearSelection();
	void selectAll();
	void deleteSelected();
	void invertSelection();

	[[nodiscard]] std::vector<std::shared_ptr<Model>>& getSelectedModels();
	[[nodiscard]] std::vector<std::shared_ptr<Model>>& getModels();

	[[nodiscard]] Vec3& getMutRefBackgroundColour();

	[[nodiscard]] const Texture& getTexture(const std::string& path);

	[[nodiscard]] const char* getPath();
	void setName(const char* name);

	void render();
	void handleMouseClicks();
}
