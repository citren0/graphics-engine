# graphics engine
This is a primitive C++ rendering graphics engine.\
It requires a running X11 server, and OpenCL.

The program is currently set up to read the .obj file in the root directory of the repository, and clone it, creating 100 copies of the object.\
The current object is a cat, and it will be cloned into a small army.

## Principles
This graphics engine relies on the algebraic concept of coordinate transformations.\
The vertices of all objects displayed are kept in a matrix, a vector of vectors. Each point has an X, Y, Z, and homogeneous coordinate.\
These coordinates are transformed by other matrices to perform simple operations like translation and rotation.\
By transforming all objects in the coordinate space, the graphics engine mimicks movement of the camera, which is actually locked at the XYZ coordinate (0, 0, 0).

![image](https://github.com/citren0/graphics-engine/assets/42524057/c8cb379c-8c71-4c73-96db-edb381da6b9d)\
Source: [https://courses.cs.washington.edu/courses/cse557/97wi/notes/xforms/projective.html](https://courses.cs.washington.edu/courses/cse557/97wi/notes/xforms/projective.html)\
This is an intuitive visual which shows the "screen" at $w$ and a ray from the origin (camera) to the object which is being projected.

This perspective projection is achieved through the "transformation matrix."
![image](https://github.com/citren0/graphics-engine/assets/42524057/0fe789f4-356a-433c-89c4-a36391e3eaef)\
This transformation is applied to all vertices and results in their coordinates projected upon the "screen" at $w = 1$

Further information can be found at my blog: [blog.cam-davis.com](blog.cam-davis.com)

## Build
$ make

## Run
$ ./a.out
