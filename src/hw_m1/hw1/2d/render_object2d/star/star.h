#pragma once

#include "hw_m1/hw1/2d/render_object2d/render_object2d.h"
#include "utils/glm_utils.h"

namespace m1
{
    class Star : public RenderObject2D {
    public:
        Star();
        Star(glm::vec3 color);
        virtual ~Star();

        virtual Star* Create() override;
        virtual Star* Clone() override;

    public:
        virtual void Init() override;
    };
}
