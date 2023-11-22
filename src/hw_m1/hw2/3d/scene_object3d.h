#pragma once

#include <vector>

namespace m1
{
    enum Object3DState { ACTIVE, STOPPING, INACTIVE };

    struct SceneObject3D {
        SceneObject3D();

        void Reset();

        bool enabled;
        Object3DState state;
    };
}
