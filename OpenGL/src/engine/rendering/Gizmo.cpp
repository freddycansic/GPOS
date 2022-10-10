#include "Gizmo.h"

#include "objects/Line.h"
#include "objects/Cube.h"

constexpr float GIZMO_WIDTH = 0.02f;

ScaleGizmo::ScaleGizmo(float x, float y, float z)
{
	static constexpr float GIZMO_SCALE_HANDLE_SIZE = 0.1f;
	
	shapes.emplace_back(std::make_unique<Line>(0, 0, 0, 1, 0, 0, GIZMO_WIDTH), Vec4(1, 0, 0, 1));
	shapes.emplace_back(std::make_unique<Line>(0, 0, 0, 0, -1, 0, GIZMO_WIDTH), Vec4(0, 1, 0, 1));
	shapes.emplace_back(std::make_unique<Line>(0, 0, 0, 0, 0, 1, GIZMO_WIDTH), Vec4(0, 0, 1, 1));

	shapes.emplace_back(std::make_unique<Cube>(1, 0, 0, GIZMO_SCALE_HANDLE_SIZE), Vec4(1, 0, 0, 1));
	shapes.emplace_back(std::make_unique<Cube>(0, -1, 0, GIZMO_SCALE_HANDLE_SIZE), Vec4(0, 1, 0, 1));
	shapes.emplace_back(std::make_unique<Cube>(0, 0, 1, GIZMO_SCALE_HANDLE_SIZE), Vec4(0, 0, 1, 1));
}

ScaleGizmo::ScaleGizmo(const Vec3& pos) : ScaleGizmo(pos.x, pos.y, pos.z) {}

TranslateGizmo::TranslateGizmo(float x, float y, float z)
{

}

TranslateGizmo::TranslateGizmo(const Vec3& pos) : TranslateGizmo(pos.x, pos.y, pos.z)
{
}

RotateGizmo::RotateGizmo(float x, float y, float z)
{

}

RotateGizmo::RotateGizmo(const Vec3& pos) : RotateGizmo(pos.x, pos.y, pos.z)
{
}