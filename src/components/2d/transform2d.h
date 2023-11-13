#pragma once

#include "utils/glm_utils.h"

#include <vector>

namespace gfxc
{
    class Transform2D {
    public:
        Transform2D();
        Transform2D(Transform2D* parent);

        glm::mat3 GetModelMatrix();
        void ComputeModelMatrix();

        void SetPosition(glm::vec2 pos);
        void AddPosition(glm::vec2 qty);
        glm::vec2 GetPosition();

        void SetScale(glm::vec2 scale);
        void AddScale(glm::vec2 qty);
        glm::vec2 GetScale();

        void SetRotation(float rotation);
        void AddRotation(float qty);
        float GetRotation();

        void AddChildren(Transform2D* children);

    protected:
        std::vector<Transform2D*> childrens;
        Transform2D* parent;

        glm::mat3 modelMatrix;

        glm::vec2 scale;
        glm::vec2 pos;
        float rotation;

        bool dirtyFlag;
    };
}
