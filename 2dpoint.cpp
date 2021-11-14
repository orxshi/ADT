#include "kdt.h"

using namespace Kdt;

int main()
{
    Vector A{0.5, 0.5};
    Vector B{0.3, 0.7};
    Vector C{0.6, 0.4};
    Vector D{0.25, 0.1};
    Vector E{0.9, 0.8};
    Vector F{0.2, 0.9};
    Vector G{0.8, 0.3};

    Dimension dim_A{A, A};
    Dimension dim_B{B, B};
    Dimension dim_C{C, C};
    Dimension dim_D{D, D};
    Dimension dim_E{E, E};
    Dimension dim_F{F, F};
    Dimension dim_G{G, G};

    Input input;
    input.push_back(Object(0, dim_A));
    input.push_back(Object(1, dim_B));
    input.push_back(Object(2, dim_C));
    input.push_back(Object(3, dim_D));
    input.push_back(Object(4, dim_E));
    input.push_back(Object(5, dim_F));
    input.push_back(Object(6, dim_G));

    Vector box_min{0.0, 0.0};
    Vector box_max{1.0, 1.0};

    Dimension box{box_min, box_max};

    Kdt::Kdt tree(input, box, true);
    tree.dot();
    tree.print_space();

    return 0;
}
