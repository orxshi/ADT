#include "kd.h"

namespace Kd
{
    Kd::Kd(const Input& objects):
        root(nullptr)
    {
        init_root(objects);
        insert(objects);
    }

    Kd::Kd(const Input& objects, Dimension dim):
        root(nullptr)
    {
        init_root(objects, dim);
        insert(objects);
    }
    
    //Dimension::Dimension(Vector min, Vector max):
        //min(min),
        //max(max)
    //{
    //}

    Dimension bounding_box(const Input& objects)
    {
        Dimension dim;

        for (const Object& obj: objects)
        {
            for (int i=0; i<NDIM; ++i)
            {
                dim.min[i] = std::min(dim.min[i], obj.dim.min[i]);
                dim.max[i] = std::max(dim.max[i], obj.dim.max[i]);
            }
        }

        return dim;
    }

    void Kd::init_root(const Input& objects, Dimension dim)
    {
        root = new Node(0, objects.front(), dim);
        id_address.insert(std::make_pair(objects.front().id, root));
    }

    void Kd::init_root(const Input& objects)
    {
        root = new Node(0, objects.front(), bounding_box(objects));
        id_address.insert(std::make_pair(objects.front().id, root));
    }

    void Kd::insert(const Object& obj)
    {
        int count = id_address.count(obj.id);
        if (count != 0) return;

        Node* node = root->insert(obj);
        if (node != nullptr)
        {
            id_address.insert(std::make_pair(obj.id, node));
        }
    }

    void Kd::insert(const Input& objects)
    {
        for (const Object& obj: objects)
        {
            insert(obj);
        }
    }

    void dot_(std::ofstream& out, Node* node)
    {
        if (node->left != nullptr)
        {
            out << node->obj.id;
            out << " -- ";
            out << node->left->obj.id;
            out << ";\n";

            dot_(out, node->left);
        }
        if (node->right != nullptr)
        {
            out << node->obj.id;
            out << " -- ";
            out << node->right->obj.id;
            out << ";\n";

            dot_(out, node->right);
        }
    }

    void Kd::dot()
    {
        std::ofstream out;
        out.open("graph.dot");
        out << "graph G {\n";
        assert(root != nullptr);
        dot_(out, root);
        out << "}";
        out.close();
    }
}
