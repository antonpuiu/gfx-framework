#pragma once

#include "hw_m1/hw1/2d/render_object2d/render_object2d.h"
#include "utils/glm_utils.h"

namespace m1
{
    class Square : public RenderObject2D {
    public:
        Square();
        Square(glm::vec3 color, bool fill);
        Square(Square&);

        virtual ~Square();

        virtual Square* Create() override;
        virtual Square* Clone() override;

        void SetFill(bool fill);

    public:
        virtual void Init() override;

    private:
        bool fill;
    };
}
