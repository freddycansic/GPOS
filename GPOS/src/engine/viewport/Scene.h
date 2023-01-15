#pragma once

#include <memory>
#include <concepts>

#include "engine/rendering/gui/Gizmo.h"
#include "engine/rendering/objects/Object.h"
#include "engine/rendering/opengl/Texture.h"

struct Ray;
struct Vec3;

namespace Scene
{
	void init();

	void addObject(std::unique_ptr<Object>&& objectPtr);

	template <typename T, typename... Args> requires std::derived_from<T, Object>
	void addObject(const Args&... args)
	{
		addObject(std::move(std::make_unique<T>(args...)));
	}

	void duplicateCurrentSelected();
	void loadModelIntoScene();

	void saveToFile();

	void selectObject(Object* obj);
	void clearSelection();
	void selectAll();
	void deleteSelected();
	void invertSelection();

	[[nodiscard]] const std::vector<Object*>& getSelectedObjects();
	[[nodiscard]] const std::vector<std::unique_ptr<Object>>& getObjects();

	[[nodiscard]] Vec3& getMutRefBackgroundColour();

	[[nodiscard]] const Texture& getTexture(const char* path);

	[[nodiscard]] const char* getPath();
	void setName(const char* name);

	void render();
	void handleMouseClicks();
}
