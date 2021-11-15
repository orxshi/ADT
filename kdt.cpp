#include "kdt.h"

namespace Kdt
{
    Kdt::Kdt(Input& objects, Dimension dim, bool median)
    {
        root = new Node(0, dim);
        root->insert(objects, id_address, median);
    }

    bool overlap(Dimension a, Dimension b)
    {
        for (int i=0; i<NDIM; ++i)
        {
            if ((a.min[i] - b.max[i]) > ZERO) return false;
            if ((b.min[i] - a.max[i]) > ZERO) return false;
        }

        return true;
    }

    void print_search_result(const std::map<int, Node*>& id_address, std::vector<int> id)
    {
        std::ofstream out;
        out.open("search.txt");

        for (int i: id)
        {
            auto node = id_address.at(i);

            out << node->obj.id;
            out << " ";
            out << node->obj.dim.min[0];
            out << " ";
            out << node->obj.dim.min[1];
            out << " ";
            out << node->obj.dim.min[2];
            out << " ";
            out << node->obj.dim.max[0];
            out << " ";
            out << node->obj.dim.max[1];
            out << " ";
            out << node->obj.dim.max[2];
            out << "\n";
        }

        out.close();
    }

    void print_target(const Dimension& target)
    {
        std::ofstream out;
        out.open("target.txt");

        out << 99;
        out << " ";
        out << target.min[0];
        out << " ";
        out << target.min[1];
        out << " ";
        out << target.min[2];
        out << " ";
        out << target.max[0];
        out << " ";
        out << target.max[1];
        out << " ";
        out << target.max[2];
        out << "\n";

        out.close();
    }

    std::vector<int> Kdt::search(const Dimension& target)
    {
        std::vector<int> id;

        if (root == nullptr) return id;

        root->search(target, id);

        print_search_result(id_address, id);
        print_target(target);

        return id;
    }
    
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

    void Kdt::dot()
    {
        std::ofstream out;
        out.open("graph.dot");
        out << "graph G {\n";
        assert(root != nullptr);
        dot_(out, root);
        out << "}";
        out.close();
    }

    void Kdt::print_space()
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
                out << node->dim.min[2];
                out << " ";
                out << node->dim.max[0];
                out << " ";
                out << node->dim.max[1];
                out << " ";
                out << node->dim.max[2];
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
                out << " ";
                out << node->obj.dim.min[2];
                out << " ";
                out << node->obj.dim.max[0];
                out << " ";
                out << node->obj.dim.max[1];
                out << " ";
                out << node->obj.dim.max[2];
                out << "\n";
            }

            out.close();
        }
    }

}
