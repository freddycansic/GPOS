#include "Object.h"

Object::Object(std::unique_ptr<Shape>&& shapePtr, const Material& material)
	:shapePtr(std::move(shapePtr)), material(material)
{
}