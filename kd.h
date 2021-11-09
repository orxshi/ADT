#ifndef kd_h
#define kd_h

#include <array>
#include <vector>
#include <map>
#include <fstream>
#include <cassert>

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
        //Dimension(Vector min, Vector max);
    };

    struct Object
    {
        Object(int id, Dimension dim);
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
        int key;
        int level;

        Node(int level, const Object& obj, Dimension dim);
        Node* insert(const Object& obj);
        bool insert_left(const Object& obj);
        bool insert_right(const Object& obj);
    };

    struct Kd
    {
        Node* root;
        std::map<int, Node*> id_address;

        void init_root(const Input& objects);
        bool insert(const Object& obj);
        void insert(const Input& objects);
        void dot();

        Kd(const Input& objects);
    };

}

#endif
