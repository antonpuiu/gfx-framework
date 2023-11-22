#include "scene_object3d.h"

using namespace m1;

SceneObject3D::SceneObject3D()
    : enabled(true)
    , state(Object3DState::ACTIVE)
{
}

void SceneObject3D::Reset()
{
    enabled = true;
    state = Object3DState::ACTIVE;
}
