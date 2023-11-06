#include "components/transform2d.h"

using namespace gfxc;

Transform2D::Transform2D()
{
}

glm::mat3 Transform2D::GetModelMatrix()
{
    return modelMatrix;
}

void Transform2D::ComputeModelMatrix()
{
}

void Transform2D::ComputeModelMatrix(glm::mat3 parentGlobalModelMatrix)
{
}

void Transform2D::setPosition(glm::vec2 pos)
{
}

void Transform2D::addPosition(glm::vec2 qty)
{
}

glm::vec2 Transform2D::getPosition()
{
}

void Transform2D::setScale(glm::vec2 scale)
{
}

void Transform2D::addScale(glm::vec2 qty)
{
}

glm::vec2 Transform2D::getScale()
{
}

void Transform2D::setRotation(float rotation)
{
}

void Transform2D::addRotation(float qty)
{
}

float Transform2D::getRotation()
{
}

void Transform2D::GetLocalModelMatrix()
{
}
