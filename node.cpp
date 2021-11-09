#include "kd.h"

namespace Kd
{
    Node::Node(int level, const Object& obj, Dimension dim):
        left(nullptr),
        right(nullptr),
        level(level),
        obj(obj),
        dim(dim)
    {
        axis = level % NDIM;
        key = 0.5 * (dim.min[axis] + dim.max[axis]);
    }

    Node* Node::insert(const Object& obj)
    {
        Node* node = nullptr;

        if (obj.dim.min[axis] < key)
        {
            bool inserted = insert_left(obj);
            if (inserted) return left;
            node = left->insert(obj);
        }
        else
        {
            bool inserted = insert_right(obj);
            if (inserted) return right;
            node = right->insert(obj);
        }

        return node;
    }

    bool Node::insert_left(const Object& obj)
    {
        if (left == nullptr)
        {
            Dimension half_dim = dim;
            half_dim.max[axis] = key;

            left = new Node(level + 1, obj, dim);
            return true;
        }

        return false;
    }

    bool Node::insert_right(const Object& obj)
    {
        if (right == nullptr)
        {
            Dimension half_dim = dim;
            half_dim.min[axis] = key;

            right = new Node(level + 1, obj, dim);
            return true;
        }

        return false;
    }
}
