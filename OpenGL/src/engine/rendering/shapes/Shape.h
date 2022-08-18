#pragma once

#include <vector>

#include "maths/Transform.h"

class Shape
{
protected:
	/**
	* @brief Current transform from the unit shape. 
	**/
	Transform m_Transform;

	/**
	 * @brief All vertices that the shape comprises of.
	*/
	std::vector<Vec3> m_Positions;

	bool m_Moved = true;

public:
	Shape() = default;
	virtual ~Shape() = default;

	/**
	 * @brief Recalculates the positions of a shape's vertices using values from its m_Transform.
	*/
	virtual void recalculatePositions();

	/**
	 * @return A const reference to a vector of the shape's indices.
	*/
	[[nodiscard]] virtual const std::vector<unsigned int>& getIndices() const = 0;

	/**
	 * @return A const reference to a vector of the shape's unit positions.
	*/
	[[nodiscard]] virtual const std::vector<Vec3>& getUnitPositions() const = 0;

	/**
	 * @return A const reference to a vector of the shape's texture coordinates.
	*/
	[[nodiscard]] virtual const std::vector<Vec2>& getTextureCoordinates() const = 0;

	/**
	 * @return A reference to a vector of the shape's positions.
	*/
	[[nodiscard]] virtual std::vector<Vec3> getPositions() const;

	/**
	 * @brief Set the scale for the shape.
	 * @param xScale The scale on the x axis
	 * @param yScale The scale on the y axis
	 * @param zScale The scale on the z axis
	*/
	virtual void setScale(float xScale, float yScale, float zScale);
	
	/**
	 * @brief Set the rotation for the shape about the centre of mass of the shape.
	 * @param xRotation The rotation on the x axis
	 * @param yRotation The rotation on the y axis
	 * @param zRotation The rotation on the z axis
	*/
	virtual void setRotation(float xRotation, float yRotation, float zRotation);
	
	/**
	 * @brief Set the translation for the shape about the origin.
	 * @param xTranslate The translation on the x axis
	 * @param yTranslate The translation on the y axis
	 * @param zTranslate The translation on the z axis
	*/
	virtual void setTranslation(float xTranslate, float yTranslate, float zTranslate);
};