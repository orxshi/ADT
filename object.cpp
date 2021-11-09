#include "kd.h"

namespace Kd
{
    Object::Object(int id, Dimension dim): id(id), dim(dim)
    {
    }

    bool Object::overlap(const Object& obj) const
    {
        for (int i=0; i<NDIM; ++i)
        {
            if ((dim.min[i] - obj.dim.max[i]) > ZERO) return false;
        }
        
        for (int i=0; i<NDIM; ++i)
        {
            if ((obj.dim.min[i] - dim.max[i]) > ZERO) return false;
        }

        return true;
    }
}
