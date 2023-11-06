#pragma once

#include "utils/glm_utils.h"

namespace gfxc
{
    class Transform2D {
    public:
        Transform2D();

        glm::mat3 GetModelMatrix();
        void ComputeModelMatrix();
        void ComputeModelMatrix(glm::mat3 parentGlobalModelMatrix);

        void setPosition(glm::vec2 pos);
        void addPosition(glm::vec2 qty);
        glm::vec2 getPosition();

        void setScale(glm::vec2 scale);
        void addScale(glm::vec2 qty);
        glm::vec2 getScale();

        void setRotation(float rotation);
        void addRotation(float qty);
        float getRotation();

    private:
        void GetLocalModelMatrix();

    protected:
        glm::mat3 modelMatrix;

        glm::vec2 scale;
        glm::vec2 pos;
        float rotation;

        bool dirtyFlag;
    };
}
