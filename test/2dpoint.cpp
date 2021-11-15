#include "../kdt.h"

using namespace Kdt;

Input create_2D_points(int n)
{
    Input input;

    for (int i=0; i<n; ++i)
    {
        srand(i+1);
        float x = (float)rand() / (float)RAND_MAX;
        float y = (float)rand() / (float)RAND_MAX;

        Vector v = {x, y};
        Dimension d = {v, v};

        input.push_back(Object(i, d));
    }

    return input;
}

int main()
{
    auto input = create_2D_points(10);

    Vector box_min{0.0, 0.0};
    Vector box_max{1.0, 1.0};

    Dimension box{box_min, box_max};

    Kdt::Kdt tree(input, box, true, 2);
    tree.dot();
    tree.print_space();

    return 0;
}
