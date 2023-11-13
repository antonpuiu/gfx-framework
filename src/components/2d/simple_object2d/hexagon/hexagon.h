#pragma once

#include "components/2d/simple_object2d/simple_object2d.h"
#include "utils/glm_utils.h"

namespace gfxc
{
    class Hexagon : public SimpleObject2D {
    public:
        Hexagon();
        Hexagon(glm::vec3 color);
        virtual ~Hexagon();

    public:
        virtual void Init() override;

    private:
        glm::vec3 color;
    };
}
