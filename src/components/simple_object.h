#pragma once

#include "core/gpu/mesh.h"

namespace gfxc
{
    class SimpleObject {
    public:
        SimpleObject();
        virtual ~SimpleObject();

        Mesh* GetMesh();

    public:
        virtual void Init() = 0;

    protected:
        Mesh* mesh;
    };
}
