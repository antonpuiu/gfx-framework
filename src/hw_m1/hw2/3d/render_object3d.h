#pragma once

#include "core/gpu/mesh.h"
#include "hw_m1/hw2/3d/transform3d.h"

namespace m1
{
    class RenderObject3D : public Transform3D {
    public:
        RenderObject3D();
        RenderObject3D(glm::vec3 color);
        RenderObject3D(RenderObject3D&);

        glm::vec3 GetColor();
        void SetColor(glm::vec3 color);
        Mesh* GetMesh();

    public:
        virtual ~RenderObject3D();

    public:
        virtual void Init() = 0;
        virtual RenderObject3D* Create() = 0;
        virtual RenderObject3D* Clone() = 0;

    protected:
        Mesh* mesh;
        glm::vec3 color;
    };

    class Cube : public RenderObject3D {
    public:
        Cube();
        Cube(glm::vec3 color);
        Cube(Cube&);

    public:
        virtual RenderObject3D* Create() override;
        virtual RenderObject3D* Clone() override;
        virtual void Init() override;
    };

    class Cylinder : public RenderObject3D {
    public:
        Cylinder();
        Cylinder(glm::vec3 color);
        Cylinder(Cylinder&);

    public:
        virtual RenderObject3D* Create() override;
        virtual RenderObject3D* Clone() override;
        virtual void Init() override;
    };

    class Sphere : public RenderObject3D {
    public:
        Sphere();
        Sphere(glm::vec3 color);
        Sphere(Sphere&);

    public:
        virtual RenderObject3D* Create() override;
        virtual RenderObject3D* Clone() override;
        virtual void Init() override;
    };
}
