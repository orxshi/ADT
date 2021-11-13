#ifndef kdt_h
#define kdt_h

#include <array>
#include <vector>
#include <map>
#include <fstream>
#include <cassert>
#include <iostream>
#include <algorithm>

namespace Kdt
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
        Object(int, Dimension dim);
        bool overlap(const Object& obj) const;

        int id;
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
        Node(int level, Object obj, Dimension dim, bool median);
        //Node* insert(Input& objects, bool median);
        //bool insert_left(const Object& obj, bool median);
        //bool insert_right(const Object& obj, bool median);
        Dimension half_dim_left(bool median);
        Dimension half_dim_right(bool median);
        void insert(Input objects, std::map<int, Node*>& id_address, bool median);
    };

    struct Kdt
    {
        Node* root;
        std::map<int, Node*> id_address;

        //void init_root(const Input& objects);
        void init_root(Input& objects, Dimension dim, bool median);
        void insert(Input& objects, bool median);
        void dot();
        void print_space();

        //Kdt(const Input& objects);
        Kdt(Input& objects, Dimension dim, bool median);
    };

    int find_median(int axis, Input& objects);
}

#endif
