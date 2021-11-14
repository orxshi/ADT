#include "kdt.h"

namespace Kdt
{
    int find_median(int axis, Input& objects)
    {
        std::sort(objects.begin(), objects.end(), [&](Object& a, Object& b){return a.dim.min[axis] < b.dim.min[axis];});
        int size = objects.size();

        if (size % 2 != 0)
        {
            return (size + 1) / 2 - 1;
        }

        return size / 2;
    }

    Node::Node(int level, Object obj, Dimension dim, bool median):
        left(nullptr),
        right(nullptr),
        level(level),
        obj(obj),
        dim(dim)
    {
        axis = level % NDIM;
        set_key(median);
    }

    Node::Node(int level, Dimension dim):
        left(nullptr),
        right(nullptr),
        level(level),
        dim(dim)
    {
        axis = level % NDIM;
    }

    Dimension Node::half_dim_left(bool median)
    {
        Dimension half_dim = dim;

        if (median)
        {
            half_dim.max[axis] = this->obj.dim.min[axis];
        }
        else
        {
            half_dim.max[axis] = key;
        }

        return half_dim;
    }

    Dimension Node::half_dim_right(bool median)
    {
        Dimension half_dim = dim;

        if (median)
        {
            half_dim.min[axis] = this->obj.dim.min[axis];
        }
        else
        {
            half_dim.min[axis] = key;
        }

        return half_dim;
    }

    void Node::set_key(bool median)
    {
        if (median)
        {
            key = obj.dim.min[axis];
        }
        else
        {
            key = 0.5 * (dim.min[axis] + dim.max[axis]);
        }
    }

    void Node::insert(Input objects, std::map<int, Node*>& id_address, bool median)
    {
        if (objects.empty())
        {
            return;
        }

        int index;

        if (median)
        {
            index = find_median(axis, objects);
        }
        else
        {
            index = 0;
        }

        const Object& obj = objects[index];

        this->obj = obj;
        set_key(median);
        id_address.insert(std::make_pair(obj.id, this));

        auto slice_left = std::vector<Object>(objects.begin(), objects.begin() + index);
        auto slice_right = std::vector<Object>(objects.begin() + index + 1, objects.end());

        if (!slice_left.empty())
        {
            assert(left == nullptr);
            left = new Node(level + 1, half_dim_left(median));
            left->insert(slice_left, id_address, median);
        }

        if (!slice_right.empty())
        {
            assert(right == nullptr);
            right = new Node(level + 1, half_dim_right(median));
            right->insert(slice_right, id_address, median);
        }
    }
}
