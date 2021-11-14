# k-d tree

A k-d tree which can insert hexahedra into partitioned 3D space with cycling cutting axes. Input objects can also be point or rectangle. Tree in balanced by inserting the median object in each cutting dimension.

VTK can be used to visualize the space and objects. Related files are in vtk folder.

A graph can be produced dot utility.

You can find three tests in tests folder.

## Example: Test 1: Points in 2D space

```
cd test
cmake .
make
./2dpoint
dot -Tpng graph.dot > graph.png
display graph.png
```

<img src="/image/2dpoint.png" width="500"/>
<img src="/image/2dpoint_graph.png" width="500"/>

## TODO

* Search operation will be added in few days
