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
        virtual Plane* Create() override;
        virtual Plane* Clone() override;
    };

    class TankBody : public Object3D {
    public:
        virtual void InitObject() override;
        virtual TankBody* Create() override;
        virtual TankBody* Clone() override;
    };

    class TankTurret : public Object3D {
    public:
        virtual void InitObject() override;
        virtual TankTurret* Create() override;
        virtual TankTurret* Clone() override;
    };

    class TankProjectile : public Object3D {
    public:
        virtual void InitObject() override;
        virtual TankProjectile* Create() override;
        virtual TankProjectile* Clone() override;
    };

    class TankGun : public Object3D {
    public:
        virtual void InitObject() override;
        virtual TankGun* Create() override;
        virtual TankGun* Clone() override;
    };

    class TankTracks : public Object3D {
    public:
        virtual void InitObject() override;
        virtual TankTracks* Create() override;
        virtual TankTracks* Clone() override;
    };

    class Tank : public Object3D {
    public:
        virtual void InitObject() override;
        virtual Tank* Create() override;
        virtual Tank* Clone() override;
    };
}
