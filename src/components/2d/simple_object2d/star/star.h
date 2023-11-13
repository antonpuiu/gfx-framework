#pragma once

#include "components/2d/simple_object2d/simple_object2d.h"
#include "utils/glm_utils.h"

namespace gfxc
{
    class Star : public SimpleObject2D {
    public:
        Star();
        Star(glm::vec3 color);
        virtual ~Star();

    public:
        virtual void Init() override;

    private:
        glm::vec3 color;
    };
}
