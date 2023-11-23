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

    protected:
        Mesh* mesh;
        glm::vec3 color;
    };

    class Cube : public RenderObject3D {
    public:
        Cube();
        Cube(glm::vec3 color);

    public:
        virtual void Init() override;
    };

    class CylinderBase : public RenderObject3D {
    public:
        CylinderBase();
        CylinderBase(glm::vec3 color);

    protected:
        static inline int RESOLUTION = 250;
        static inline float OFFSET = 0;
    };

    class CylinderCover : public CylinderBase {
    public:
        CylinderCover();
        CylinderCover(glm::vec3 color);

    public:
        virtual void Init() override;
    };

    class CylinderBody : public CylinderBase {
    public:
        CylinderBody();
        CylinderBody(glm::vec3 color);

    public:
        virtual void Init() override;
    };

    class Sphere : public RenderObject3D {
    public:
        Sphere();
        Sphere(glm::vec3 color);

    public:
        virtual void Init() override;

    private:
        static inline int RESOLUTION = 50;
    };
}
