#pragma once

#include "hw_m1/hw1/2d/render_object2d/render_object2d.h"
#include "utils/glm_utils.h"

namespace m1
{
    class Diamond : public RenderObject2D {
    public:
        Diamond();
        Diamond(glm::vec3 color);
        virtual ~Diamond();

        virtual Diamond* Create() override;
        virtual Diamond* Clone() override;

    public:
        virtual void Init() override;
    };
}
