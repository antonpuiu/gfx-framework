#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace object2D
{
    enum ObjectType { SQUARE, SQUARE_FRAME, STAR, HEXAGON };

    Mesh* CreateObject(ObjectType type, const std::string& name, glm::vec3 color);
}
