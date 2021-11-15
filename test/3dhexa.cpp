#include "../kdt.h"

using namespace Kdt;

int main()
{
    Dimension dim_0{Vector{0.1, 0.1, 0.1}, Vector{0.3, 0.3, 0.3}};
    Dimension dim_1{Vector{0.6, 0.6, 0.6}, Vector{0.7, 0.7, 0.7}};
    Dimension dim_2{Vector{0.65, 0.63, 0.63}, Vector{0.75, 0.5, 0.5}};
    Dimension dim_3{Vector{0.45, 0.27, 0.27}, Vector{0.65, 0.1, 0.1}};

    Input input;
    input.push_back(Object(0, dim_0));
    input.push_back(Object(1, dim_1));
    input.push_back(Object(2, dim_2));
    input.push_back(Object(3, dim_3));

    Vector box_min{0.0, 0.0, 0.0};
    Vector box_max{1.0, 1.0, 1.0};

    Dimension box{box_min, box_max};

    Kdt::Kdt tree(input, box, true, 3);
    tree.dot();
    tree.print_space();

    return 0;
}
