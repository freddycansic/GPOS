#include "engine/rendering/Vertex.h"

std::ostream& operator<<(std::ostream& os, const Vertex& v)
{
	return os << "Pos(" << v.position.x << ", " << v.position.y << ", " << v.position.z << ") "
		<< "Color(" << v.color.x << ", " << v.color.y << ", " << v.color.z << ", " << v.color.w << ") "
		<< "TexCoords(" << v.textureCoordinates.x << ", " << v.textureCoordinates.y << ") "
		<< "TexID(" << v.texHandle << ")";
}
