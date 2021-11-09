#ifndef kd_h
#define kd_h

#include <array>
#include <vector>
#include <map>
#include <fstream>
#include <cassert>
#include <iostream>

namespace Kd
{
    const int NDIM = 3;
    const double ZERO = 1e-9;
    const double BIG_POS_NUM = 1e9;
    const double BIG_NEG_NUM = -1e9;

    using Vector = std::array<double, NDIM>;

    struct Dimension
    {
        Vector min;
        Vector max;
    };

    struct Object
    {
        Object(std::string id, Dimension dim);
        bool overlap(const Object& obj) const;

        std::string id;
        Dimension dim;
    };

    using Input = std::vector<Object>;

    struct Node
    {
        Object obj;
        Node* left;
        Node* right;
        Dimension dim;
        int axis;
        double key;
        int level;

        Node(int level, const Object& obj, Dimension dim);
        Node* insert(const Object& obj);
        bool insert_left(const Object& obj);
        bool insert_right(const Object& obj);
    };

    struct Kd
    {
        Node* root;
        std::map<std::string, Node*> id_address;

        void init_root(const Input& objects);
        void init_root(const Input& objects, Dimension dim);
        void insert(const Object& obj);
        void insert(const Input& objects);
        void dot();

        Kd(const Input& objects);
        Kd(const Input& objects, Dimension dim);
    };

}

#endif
