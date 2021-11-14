# k-d tree

A k-d tree which can partition three-dimensional space to organize/insert objects. The input object can be a point, rectangle and hexahedron. Insertion of 0D+ objects such as rectangle and hexahedron is managed by changing the cutting axis in cyclic order. The k-d tree in balanced by inserting the median object in each cutting dimension. This kind of k-d tree is also called Alternating Digital Tree.

The k-d tree stores IDs of the inserted objects. When a query point (can be 0D+ such as hexahedron) is given in search operation, the tree returns possibly multiple IDs of objects which overlap the query point. The user can perform actual geometric tests from thereon.

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

Display the graph with

```
dot -Tpng graph.dot > graph.png
display graph.png
```

Visualize interactively using VTK with

```
cd ../vtk
cmake .
make
./draw
```

Here are the images of the graph and the partitioned space.

<img src="/image/2dpoint_graph.png" width="500"/>
<img src="/image/2dpoint.png" width="500"/>

## TODO

* Search operation will be added in few days
* The search operation will return possibly multiple objects which overlap the query object.
* Actual geometric 
