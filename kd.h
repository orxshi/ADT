#ifndef kd_h
#define kd_h

#include <array>
#include <vector>
#include <map>
#include <fstream>
#include <cassert>
#include <iostream>
#include <algorithm>

namespace Kd
{
    const int NDIM = 2;
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
        Object() = default;
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

        Node(int level, Dimension dim);
        Node(int level, Object obj, Dimension dim);
        Node* insert(Input& objects);
        bool insert_left(const Object& obj);
        bool insert_right(const Object& obj);
    };

    struct Kd
    {
        Node* root;
        std::map<std::string, Node*> id_address;

        //void init_root(const Input& objects);
        void init_root(Input& objects, Dimension dim);
        void insert(Input& objects);
        void dot();
        void print_space();

        //Kd(const Input& objects);
        Kd(Input& objects, Dimension dim);
    };

    int find_median(int axis, Input& objects);
}

#endif
