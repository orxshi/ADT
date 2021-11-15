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
        Object() = default;
        Object(int, Dimension dim);

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
        Dimension half_dim_left(bool median);
        Dimension half_dim_right(bool median);
        void insert(Input objects, std::map<int, Node*>& id_address, bool median);
        void search(const Dimension& target, std::vector<int>& id);
        void set_key(bool median);
    };

    struct Kdt
    {
        Node* root;
        std::map<int, Node*> id_address;

        void dot();
        void print_space();
        std::vector<int> search(const Dimension& target);

        Kdt(Input& objects, Dimension dim, bool median);
    };

    int find_median(int axis, Input& objects);
    bool overlap(Dimension a, Dimension b);
}

#endif
