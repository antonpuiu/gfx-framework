#include "simple_object2d.h"

using namespace gfxc;

SimpleObject2D::SimpleObject2D()
    : Transform2D(nullptr)
{
}

SimpleObject2D::SimpleObject2D(Transform2D* parent)
    : Transform2D(parent)
{
}

SimpleObject2D::~SimpleObject2D()
{
}
