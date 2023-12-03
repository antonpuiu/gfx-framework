#pragma once

#include "utils/glm_utils.h"

#include <vector>

namespace m1
{
    class Transform3D {
    public:
        Transform3D();
        Transform3D(Transform3D* parent);
        Transform3D(Transform3D&);

        glm::mat4 GetModelMatrix();
        void ComputeModelMatrix();

        void SetPosition(glm::vec3 pos);
        virtual void AddPosition(glm::vec3 qty);
        glm::vec3 GetPosition();
        glm::vec3 GetGlobalPosition();

        void SetScale(glm::vec3 scale);
        void AddScale(glm::vec3 qty);
        glm::vec3 GetScale();
        glm::vec3 GetGlobalScale();

        void SetRotation(glm::vec3 rotation);
        virtual void AddRotation(glm::vec3 qty);
        glm::vec3 GetRotation();
        glm::vec3 GetGlobalRotation();

        void AddChildren(Transform3D* children);

        void AttatchToParent(Transform3D* parent);
        void DetatchFromParent();

    protected:
        std::vector<Transform3D*> childrens;
        Transform3D* parent;

        glm::mat4 modelMatrix;

        glm::vec3 scale;
        glm::vec3 pos;
        glm::vec3 rotation;

        glm::vec3 scaleDetatched;
        glm::vec3 posDetatched;
        glm::vec3 rotationDetatched;

        bool dirtyFlag;
        bool detatched;
    };
}
