#include "kd.h"

namespace Kd
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

    int find_median(int axis, Dimension dim, Input& objects)
    {
        if (obj.dim.min[axis] > dim.max[axis]) return;
        if (obj.dim.min[axis] < dim.min[axis]) return;

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

    int find_median(int axis, Input& objects)
    {
        //std::vector<double> container;
        //for (const auto& obj: objects)
        //{
            //container.push_back(obj.dim.min[axis]);
        //}

        //std::sort(container.begin(), container.end());
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

    Node::Node(int level, Object obj, Dimension dim):
        left(nullptr),
        right(nullptr),
        level(level),
        obj(obj),
        dim(dim)
    {
        axis = level % NDIM;
        //key = 0.5 * (dim.min[axis] + dim.max[axis]);
        key = obj.dim.min[axis];
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


    Node* Node::insert(Input& objects)
    {
        if (left != nullptr) {
            left->insert(objects);
        }
        else
        {
        }

        if (right != nullptr) {
            right->insert(objects);
        }




        int sub_axis = (level + 1) % NDIM;
        std::cout << "resident obj: " << this->obj.id << std::endl;
        std::cout << "resident axis: " << this->axis << std::endl;
        std::cout << "resident key: " << this->key << std::endl;
        int index = find_median(sub_axis, objects);
        const Object& obj = objects[index];

        Node* node = nullptr;

        if (obj.dim.min[axis] < key)
        {
            bool inserted = insert_left(obj);
            if (inserted)
            {
                objects.erase(objects.begin() + index);
                return left;
            }
            node = left->insert(objects);
        }
        else
        {
            bool inserted = insert_right(obj);
            if (inserted)
            {
                objects.erase(objects.begin() + index);
                return right;
            }
            node = right->insert(objects);
        }

        return node;
    }

    Node* Node::insert(Input& objects)
    {
        int sub_axis = (level + 1) % NDIM;
        std::cout << "resident obj: " << this->obj.id << std::endl;
        std::cout << "resident axis: " << this->axis << std::endl;
        std::cout << "resident key: " << this->key << std::endl;
        int index = find_median(sub_axis, objects);
        const Object& obj = objects[index];

        Node* node = nullptr;

        if (obj.dim.min[axis] < key)
        {
            bool inserted = insert_left(obj);
            if (inserted)
            {
                objects.erase(objects.begin() + index);
                return left;
            }
            node = left->insert(objects);
        }
        else
        {
            bool inserted = insert_right(obj);
            if (inserted)
            {
                objects.erase(objects.begin() + index);
                return right;
            }
            node = right->insert(objects);
        }

        return node;
    }

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

    bool Node::insert_left(const Object& obj)
    {
        if (left == nullptr)
        {
            Dimension half_dim = dim;
            half_dim.max[axis] = this->obj.dim.min[axis];

            left = new Node(level + 1, obj, half_dim);
            return true;
        }

        return false;
    }

    bool Node::insert_right(const Object& obj)
    {
        if (right == nullptr)
        {
            Dimension half_dim = dim;
            half_dim.min[axis] = this->obj.dim.min[axis];

            right = new Node(level + 1, obj, half_dim);
            return true;
        }

        return false;
    }
}
