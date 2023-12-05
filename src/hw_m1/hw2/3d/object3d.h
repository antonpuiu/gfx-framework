#pragma once

#include "components/camera.h"
#include "hw_m1/hw2/3d/render_object3d.h"
#include "hw_m1/hw2/3d/scene_object3d.h"
#include "hw_m1/hw2/3d/transform3d.h"
#include "lab_m1/lab5/lab_camera.h"

#include <random>

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

    class Walls : public Object3D {
    public:
        virtual void InitObject() override;
    };

    class Cylinder : public Object3D {
    public:
        virtual void InitObject() override;
    };

    class Building : public Object3D {
    public:
        Building();

        void Strike();
        float GetHp();

    public:
        virtual void Update(float deltaTimeSeconds) override;
        virtual void InitObject() override;

    private:
        static inline std::random_device rd = std::random_device();
        static inline std::uniform_real_distribution<float>
            width = std::uniform_real_distribution<float>(1.0, 3.0),
            height = std::uniform_real_distribution<float>(1.0, 10.0),
            length = std::uniform_real_distribution<float>(1.0, 5.0);

    public:
        static inline std::default_random_engine engine =
            std::default_random_engine(Building::rd());
        static inline std::uniform_int_distribution<int> count = std::uniform_int_distribution<int>(
                                                             30, 120),
                                                         colors =
                                                             std::uniform_int_distribution<int>(
                                                                 0, VertexColor::colors.size() - 1);
        static inline std::uniform_real_distribution<float> position =
            std::uniform_real_distribution<float>(-40, 40);

    private:
        float hp;
    };

    class TankBody : public Object3D {
    public:
        virtual void InitObject() override;
    };

    class TankGun;

    class TankTurret : public Object3D {
    public:
        virtual void InitObject() override;

    private:
        TankGun* gun;

        friend class Tank;
    };

    class Tank;

    class TankProjectile : public Object3D {
    public:
        TankProjectile(Tank* tank);

        bool IsParent(Tank* tank);

    public:
        virtual void Update(float deltaTimeSeconds) override;
        virtual void InitObject() override;

    private:
        Tank* tank;
        float timeToLive;
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
        Tank();
        Tank(implemented::Camera* camera);
        ~Tank();

        void RotateTurret(float deg);
        void RotateGun(float deg);
        glm::vec3 GetGunRot();
        float GetTankRadius();
        float GetTankHP();

        void Strike();

        TankProjectile* LaunchProjectile();

    public:
        virtual void AddPosition(glm::vec3 qty) override;
        virtual void AddRotation(glm::vec3 qty) override;

        virtual void Update(float deltaTimeSeconds) override;
        virtual void InitObject() override;

    private:
        void UpdateCamera();

    private:
        static inline std::list<Tank*> instances = {};
        static inline std::random_device rd = std::random_device();

    public:
        static inline std::default_random_engine engine = std::default_random_engine(Tank::rd());
        static inline std::uniform_int_distribution<int> count =
            std::uniform_int_distribution<int>(10, 80);
        static inline std::uniform_real_distribution<float>
            position = std::uniform_real_distribution<float>(-40, 40),
            rot = std::uniform_real_distribution<float>(0, 360);

    private:
        TankTurret* turret;
        implemented::Camera* camera;
        float hp;
        float timeout;
    };
}
