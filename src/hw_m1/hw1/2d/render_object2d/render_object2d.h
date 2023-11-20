#pragma once

#include "core/gpu/mesh.h"
#include "hw_m1/hw1/2d/transform2d/transform2d.h"

namespace m1
{
    class RenderObject2D : public Transform2D {
    public:
        RenderObject2D();
        RenderObject2D(glm::vec3 color);
        RenderObject2D(RenderObject2D&);
        virtual ~RenderObject2D();

        virtual RenderObject2D* Create() = 0;
        virtual RenderObject2D* Clone() = 0;

        void SetColor(glm::vec3 color);
        Mesh* GetMesh();

    public:
        virtual void Init() = 0;

    protected:
        Mesh* mesh;
        glm::vec3 color;
    };
}
