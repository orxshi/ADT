#include "kd.h"

using namespace Kd;

int main()
{
    Vector A{0.5, 0.5, 0.0};
    Vector B{0.3, 0.7, 0.0};
    Vector C{0.6, 0.4, 0.0};
    Vector D{0.25, 0.1, 0.0};
    Vector E{0.9, 0.8, 0.0};
    Vector F{0.2, 0.9, 0.0};
    Vector G{0.8, 0.3, 0.0};

    Dimension dim_A{A, A};
    Dimension dim_B{B, B};
    Dimension dim_C{C, C};
    Dimension dim_D{D, D};
    Dimension dim_E{E, E};
    Dimension dim_F{F, F};
    Dimension dim_G{G, G};

    Input input;
    input.push_back(Object("A", dim_A));
    input.push_back(Object("B", dim_B));
    input.push_back(Object("C", dim_C));
    input.push_back(Object("D", dim_D));
    input.push_back(Object("E", dim_E));
    input.push_back(Object("F", dim_F));
    input.push_back(Object("G", dim_G));

    Vector box_min{0.0, 0.0, 0.0};
    Vector box_max{1.0, 1.0, 0.0};

    Dimension box{box_min, box_max};

    Kd::Kd tree(input, box);
    tree.dot();

    return 0;
}
