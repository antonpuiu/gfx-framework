#pragma once

#include <vector>

namespace m1
{
    enum ObjectState { ACTIVE, HIT, DISABLED };

    struct SceneObject2D {
        SceneObject2D();

        void Reset();

        bool enabled;
        ObjectState state;
    };
}
