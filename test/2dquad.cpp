#include "../kdt.h"

using namespace Kdt;

int main()
{
    Dimension dim_0{Vector{0.1, 0.1}, Vector{0.3, 0.3}};
    //Dimension dim_1{Vector{0.2, 0.1}, Vector{0.4, 0.3}};
    Dimension dim_1{Vector{0.6, 0.6}, Vector{0.7, 0.7}};

    Input input;
    input.push_back(Object(0, dim_0));
    input.push_back(Object(1, dim_1));

    Vector box_min{0.0, 0.0};
    Vector box_max{1.0, 1.0};

    Dimension box{box_min, box_max};

    Kdt::Kdt tree(input, box, true);
    tree.dot();
    tree.print_space();

    return 0;
}
