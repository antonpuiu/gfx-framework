#pragma once

#include "components/2d/simple_object2d/simple_object2d.h"
#include "utils/glm_utils.h"

namespace gfxc
{
    class Square : public SimpleObject2D {
    public:
        Square();
        Square(glm::vec3 color, bool fill);
        virtual ~Square();

    public:
        virtual void Init() override;

    private:
        glm::vec3 color;
        bool fill;
    };
}
