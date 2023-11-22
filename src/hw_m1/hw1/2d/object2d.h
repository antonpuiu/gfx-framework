#pragma once

#include "hw_m1/hw1/2d/render_object2d/render_object2d.h"
#include "hw_m1/hw1/2d/scene_object2d/scene_object2d.h"
#include "hw_m1/hw1/2d/transform2d/transform2d.h"

namespace m1
{
    class Object2D : public RenderObject2D, public SceneObject2D {
    public:
        Object2D() = default;
        Object2D(Object2D&);

    public:
        virtual ~Object2D();

        virtual void Update(float deltaTimeSeconds);

    public:
        std::vector<std::pair<Mesh*, glm::mat3> > GetRenderDetails();

    protected:
        std::vector<RenderObject2D*> objects;
    };

    class ColorObject2D : public Object2D {
    public:
        ColorObject2D(glm::vec3 color);
        ColorObject2D(ColorObject2D& colorObject2D);

        glm::vec3 GetColor();

    protected:
        glm::vec3 color;
    };

    class Limit : public Object2D {
    public:
        Limit() = default;
        Limit(Limit&);

    public:
        virtual Limit* Create() override;
        virtual Limit* Clone() override;
        virtual void Init() override;
    };

    class Projectile : public ColorObject2D {
    public:
        Projectile();
        Projectile(glm::vec3 color, int life);

    public:
        virtual Projectile* Create() override;
        virtual Projectile* Clone() override;
        virtual void Init() override;

    public:
        int life;
    };

    class Launcher : public ColorObject2D {
    public:
        Launcher();
        Launcher(glm::vec3 color, int cost);
        Launcher(Launcher&);

        int GetCost();
        Projectile* Launch();

    public:
        virtual Launcher* Create() override;
        virtual Launcher* Clone() override;
        virtual void Init() override;
        virtual void Update(float deltaTimeSeonds) override;

    private:
        static inline float RESET_TIME = 0.5;

    private:
        int cost;
        int life;
        float timeout;
    };

    class Spot : public Object2D {
    public:
        Spot();
        Spot(Spot&);

        void Attatch(Launcher* launcher);
        void Detatch();

        Launcher* GetLauncher();

    public:
        virtual Spot* Create() override;
        virtual Spot* Clone() override;
        virtual void Init() override;

    private:
        Launcher* launcher;
    };

    class Generator : public ColorObject2D {
    public:
        Generator();
        Generator(glm::vec3 color, int cost);
        Generator(Generator&);

        Launcher* Generate();
        int GetCost();

    public:
        virtual Generator* Create() override;
        virtual Generator* Clone() override;
        virtual void Init() override;

    private:
        Launcher* launcher;
        int cost;
    };

    class Enemy : public ColorObject2D {
    public:
        Enemy();
        Enemy(glm::vec3 color, int life);
        Enemy(Enemy&);

    public:
        virtual Enemy* Create() override;
        virtual Enemy* Clone() override;
        virtual void Init() override;

    public:
        int life;
    };

    class Life : public Object2D {
    public:
        virtual void Init() override;
        virtual Life* Create() override;
        virtual Life* Clone() override;
    };

    class Energy : public ColorObject2D {
    public:
        Energy();
        Energy(glm::vec3 color);

    public:
        virtual Energy* Create() override;
        virtual Energy* Clone() override;
        virtual void Init() override;
    };
}
