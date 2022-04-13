#include "engine/rendering/Vertex.h"

Vertex::Vertex(Vec3 position, Vec4 color, Vec2 textureCoordinates, float texID) :
	position(position), color(color), textureCoordinates(textureCoordinates), texID(texID)
{
}

std::ostream& operator<<(std::ostream& os, const Vertex& v)
{
	return os << "Pos(" << v.position.x << ", " << v.position.y << ", " << v.position.z << ") "
		<< "Color(" << v.color.x << ", " << v.color.y << ", " << v.color.z << ", " << v.color.w << ") "
		<< "TexCoords(" << v.textureCoordinates.x << ", " << v.textureCoordinates.y << ") "
		<< "TexID(" << v.texID << ")";
}
