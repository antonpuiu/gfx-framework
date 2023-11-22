#include "scene_object2d.h"

using namespace m1;

SceneObject2D::SceneObject2D()
    : enabled(true)
    , state(Object2DState::ENABLED)
{
}

void SceneObject2D::Reset()
{
    enabled = true;
    state = Object2DState::ENABLED;
}
