#pragma once

#include "components/simple_object.h"
#include "components/2d/transform2d.h"

namespace gfxc
{
    class SimpleObject2D : public Transform2D, public SimpleObject {
    public:
        SimpleObject2D();
        SimpleObject2D(Transform2D* parent);
        virtual ~SimpleObject2D();
    };
}
