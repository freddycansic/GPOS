#include "Transform.h"

std::ostream& operator<<(std::ostream& os, const Transform& transform) {
	os << "Transform : " << transform.tra.x << ", " << transform.tra.y << ", " << transform.tra.z << "\n" <<
		"Rotation : " << transform.rot.x << ", " << transform.rot.y << ", " << transform.rot.z << "\n" <<
		"Scale : " << transform.sca.x << ", " << transform.sca.y << ", " << transform.sca.z;
	return os;
}