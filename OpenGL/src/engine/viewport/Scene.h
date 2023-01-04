#pragma once

#include <memory>
#include <concepts>

#include "engine/rendering/gui/Gizmo.h"
#include "engine/rendering/objects/Object.h"

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

	void selectObject(Object* obj);
	void clearSelection();
	void selectAll();
	void deleteSelected();

	[[nodiscard]] const std::vector<Object*>& getSelectedObjects();
	[[nodiscard]] const std::vector<std::unique_ptr<Object>>& getObjects();

	[[nodiscard]] const Vec3& getBackgroundColour();
	void setBackgroundColour(const Vec3& colour);

	void render();
	void handleMouseClicks();
}
