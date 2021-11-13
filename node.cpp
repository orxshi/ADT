#include "kdt.h"

namespace Kdt
{
    //Node::Node(int level, const Object& obj, Dimension dim):
    //    left(nullptr),
    //    right(nullptr),
    //    level(level),
    //    obj(obj),
    //    dim(dim)
    //{
    //    axis = level % NDIM;
    //    key = 0.5 * (dim.min[axis] + dim.max[axis]);
    //}

    int find_median(int axis, Input& objects)
    {
        std::sort(objects.begin(), objects.end(), [&](Object& a, Object& b){return a.dim.min[axis] < b.dim.min[axis];});
        int size = objects.size();

        std::cout << "level: " << axis << std::endl;
        std::cout << "axis: " << axis << std::endl;
        std::cout << "size: " << size << std::endl;
        for (auto a: objects)
        {
            std::cout << "a: " << a.id << std::endl;
        }
        //assert(false);

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
        if (median)
        {
            key = obj.dim.min[axis];
        }
        else
        {
            key = 0.5 * (dim.min[axis] + dim.max[axis]);
        }
    }

    Node::Node(int level, Dimension dim):
        left(nullptr),
        right(nullptr),
        level(level),
        dim(dim)
    {
        axis = level % NDIM;
        key = 0.5 * (dim.min[axis] + dim.max[axis]);
    }

    //Node* Node::insert(Input& objects, bool median)
    //{
    //    //int sub_axis = (level + 1) % NDIM;
    //    //std::cout << "resident obj: " << this->obj.id << std::endl;
    //    //std::cout << "resident axis: " << this->axis << std::endl;
    //    //std::cout << "resident key: " << this->key << std::endl;
    //    //int index = find_median(sub_axis, objects);
    //    //const Object& obj = objects[index];
    //    const Object& obj = objects.front();

    //    Node* node = nullptr;

    //    if (obj.dim.min[axis] < key)
    //    {
    //        bool inserted = insert_left(obj, median);
    //        //if (inserted)
    //        //{
    //            //objects.erase(objects.begin() + index);
    //            //objects.erase(objects.begin());
    //            //return left;
    //        //}
    //        //node = left->insert(objects, median);
    //    }
    //    else
    //    {
    //        bool inserted = insert_right(obj, median);
    //        if (inserted)
    //        {
    //            //objects.erase(objects.begin() + index);
    //            objects.erase(objects.begin());
    //            return right;
    //        }
    //        node = right->insert(objects, median);
    //    }

    //    return node;
    //}

    //Node* Node::insert(const Object& obj)
    //{
    //    Node* node = nullptr;

    //    if (obj.dim.min[axis] < key)
    //    {
    //        bool inserted = insert_left(obj);
    //        if (inserted) return left;
    //        node = left->insert(obj);
    //    }
    //    else
    //    {
    //        bool inserted = insert_right(obj);
    //        if (inserted) return right;
    //        node = right->insert(obj);
    //    }

    //    return node;
    //}

    //bool Node::insert_left(const Object& obj, bool median)
    //{
    //    if (left == nullptr)
    //    {
    //        Dimension half_dim = dim;
    //        if (median)
    //        {
    //            half_dim.max[axis] = this->obj.dim.min[axis];
    //        }
    //        else
    //        {
    //            half_dim.max[axis] = key;
    //        }

    //        left = new Node(level + 1, obj, half_dim, median);
    //        return true;
    //    }

    //    return false;
    //}

    //bool Node::insert_right(const Object& obj, bool median)
    //{
    //    if (right == nullptr)
    //    {
    //        Dimension half_dim = dim;
    //        if (median)
    //        {
    //            half_dim.min[axis] = this->obj.dim.min[axis];
    //        }
    //        else
    //        {
    //            half_dim.min[axis] = key;
    //        }

    //        right = new Node(level + 1, obj, half_dim, median);
    //        return true;
    //    }

    //    return false;
    //}

    // assumed root is initizalized and has object.

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

    void Node::insert(Input objects, std::map<int, Node*>& id_address, bool median)
    {
        if (objects.empty())
        {
            return;
        }

        int index = find_median(axis, objects);
        const Object& obj = objects[index];

        if (obj.dim.min[axis] < key)
        {
            assert(left == nullptr);

            left = new Node(level + 1, obj, half_dim_left(median), median);
            id_address.insert(std::make_pair(obj.id, left));
            auto slice = std::vector<Object>(objects.begin(), objects.begin() + index);
            for (auto s: slice)
            {
                std::cout << "id left: " << s.id << std::endl;
            }
            left->insert(slice, id_address, median);
        }

        if (obj.dim.min[axis] >= key)
        {
            assert(right == nullptr);

            right = new Node(level + 1, obj, half_dim_right(median), median);
            id_address.insert(std::make_pair(obj.id, right));
            auto slice = std::vector<Object>(objects.begin() + index + 1, objects.end());
            for (auto s: slice)
            {
                std::cout << "id right: " << s.id << std::endl;
            }
            right->insert(slice, id_address, median);
        }
    }
}
