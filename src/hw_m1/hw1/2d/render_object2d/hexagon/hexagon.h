#pragma once

#include "hw_m1/hw1/2d/render_object2d/render_object2d.h"
#include "utils/glm_utils.h"

namespace m1
{
    class Hexagon : public RenderObject2D {
    public:
        Hexagon();
        Hexagon(glm::vec3 color);
        virtual ~Hexagon();

        Hexagon* Create() override;
        Hexagon* Clone() override;

    public:
        virtual void Init() override;
    };
}
