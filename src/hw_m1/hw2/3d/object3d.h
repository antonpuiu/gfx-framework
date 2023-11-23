#pragma once

#include "hw_m1/hw2/3d/render_object3d.h"
#include "hw_m1/hw2/3d/scene_object3d.h"
#include "hw_m1/hw2/3d/transform3d.h"

namespace m1
{

    class Object3D : public RenderObject3D, public SceneObject3D {
    public:
        Object3D();
        Object3D(Object3D&);

        std::vector<RenderObject3D*> GetPrimitives();

    public:
        virtual ~Object3D();
        virtual void Update(float deltaTimeSeconds);
        virtual void Init() override final;
        virtual void InitObject() = 0;

    protected:
        std::vector<RenderObject3D*> primitives;
        std::vector<Object3D*> objects;
    };

    class Plane : public Object3D {
    public:
        virtual void InitObject() override;
    };

    class TankBody : public Object3D {
    public:
        virtual void InitObject() override;
    };

    class TankTurret : public Object3D {
    public:
        virtual void InitObject() override;
    };

    class TankProjectile : public Object3D {
    public:
        virtual void InitObject() override;
    };

    class TankGun : public Object3D {
    public:
        virtual void InitObject() override;
    };

    class TankTracks : public Object3D {
    public:
        virtual void InitObject() override;
    };

    class Tank : public Object3D {
    public:
        virtual void InitObject() override;
    };
}
