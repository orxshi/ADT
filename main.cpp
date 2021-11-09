#include "kd.h"

using namespace Kd;

int main()
{
    Vector coord_0;
    coord_0[0] = 0.0;
    coord_0[1] = 0.0;
    coord_0[2] = 0.0;

    Vector coord_1;
    coord_1[0] = 2.0;
    coord_1[1] = 1.0;
    coord_1[2] = 3.0;

    Vector coord_2;
    coord_2[0] = 4.0;
    coord_2[1] = 0.0;
    coord_2[2] = 2.0;

    Dimension dim_0;
    dim_0.min = coord_0;
    dim_0.max = coord_0;

    Dimension dim_1;
    dim_1.min = coord_1;
    dim_1.max = coord_1;

    Dimension dim_2;
    dim_2.min = coord_2;
    dim_2.max = coord_2;

    Input input;
    input.push_back(Object(10, dim_0));
    input.push_back(Object(20, dim_1));
    input.push_back(Object(30, dim_2));

    Kd::Kd tree(input);
    tree.dot();

    return 0;
}
