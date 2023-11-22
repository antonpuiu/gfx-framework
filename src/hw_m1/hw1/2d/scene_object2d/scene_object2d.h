#pragma once

#include <vector>

namespace m1
{
    enum Object2DState { ENABLED, HIT, DISABLED };

    struct SceneObject2D {
        SceneObject2D();

        void Reset();

        bool enabled;
        Object2DState state;
    };
}
