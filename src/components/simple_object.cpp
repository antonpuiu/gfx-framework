#include "components/simple_object.h"

using namespace gfxc;

SimpleObject::SimpleObject()
    : mesh(nullptr)
{
}

SimpleObject::~SimpleObject()
{
    delete mesh;
}

Mesh* SimpleObject::GetMesh()
{
    return mesh;
}
