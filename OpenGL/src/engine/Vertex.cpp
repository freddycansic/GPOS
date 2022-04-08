#include "engine/Vertex.h"

Vertex::Vertex(Vec3 position, Vec2 textureCoordinates, float texID) :
	position(position), textureCoordinates(textureCoordinates), texID(texID)
{
}

std::ostream& operator<<(std::ostream& os, const Vertex& v)
{
	return os << "Pos(" << v.position.x << ", " << v.position.y << ", " << v.position.z << ") "
		<< "TexCoords(" << v.textureCoordinates.x << ", " << v.textureCoordinates.y << ") "
		<< "TexID(" << v.texID << ")";
}
