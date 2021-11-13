#include "kd.h"

namespace Kd
{
    //Kd::Kd(Input& objects):
    //    root(nullptr)
    //{
    //    init_root(objects);
    //    insert(objects);
    //}

    Kd::Kd(Input& objects, Dimension dim, bool median):
        root(nullptr)
    {
        init_root(objects, dim, median);
        insert(objects, median);
    }
    
    //Dimension::Dimension(Vector min, Vector max):
        //min(min),
        //max(max)
    //{
    //}
    //


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

    void Kd::init_root(Input& objects, Dimension dim, bool median)
    {
        root = new Node(0, dim);

        if (median)
        {
            int index = find_median(root->axis, objects);
            const Object& obj = objects[index];
            root->obj = obj;
            id_address.insert(std::make_pair(root->obj.id, root));
            objects.erase(objects.begin() + index);
        }
        else
        {
            root->obj = objects.front();
            id_address.insert(std::make_pair(root->obj.id, root));
            objects.erase(objects.begin());
        }
    }

    //void Kd::init_root(const Input& objects)
    //{
    //    root = new Node(0, objects.front(), bounding_box(objects));
    //    id_address.insert(std::make_pair(objects.front().id, root));
    //}

    //void Kd::insert(const Object& obj)
    //{
    //    int count = id_address.count(obj.id);
    //    assert(count == 0);
    //    //if (count != 0) return;

    //    Node* node = root->insert(objects);
    //    if (node != nullptr)
    //    {
    //        id_address.insert(std::make_pair(obj.id, node));
    //    }
    //}

    void Kd::insert(Input& objects, bool median)
    {
        while(!objects.empty())
        {
            Node* node = root->insert(objects, median);
            if (node != nullptr)
            {
                int count = id_address.count(node->obj.id);
                assert(count == 0);

                id_address.insert(std::make_pair(node->obj.id, node));
            }
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

    void Kd::print_space()
    {
        {
            std::ofstream out;
            out.open("quad.txt");

            for (auto m: id_address)
            {
                auto node = m.second;
                if (node == nullptr) continue;

                out << node->dim.min[0];
                out << " ";
                out << node->dim.min[1];
                out << " ";
                out << node->dim.max[0];
                out << " ";
                out << node->dim.max[1];
                out << "\n";
            }

            out.close();
        }

        {
            std::ofstream out;
            out.open("vertex.txt");

            for (auto m: id_address)
            {
                auto node = m.second;
                if (node == nullptr) continue;

                out << node->obj.id;
                out << " ";
                out << node->obj.dim.min[0];
                out << " ";
                out << node->obj.dim.min[1];
                out << "\n";
            }

            out.close();
        }
    }

}
