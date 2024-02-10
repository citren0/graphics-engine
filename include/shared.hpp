#pragma once

#include <X11/X.h>
#define PI 3.14159265

#define MAX_SHAPES 400000
#define MAX_VERTICES_PER_SHAPE 100
#define NUMBER_OF_HOMOGENEOUS_COORDS 4
#define MAX_CONNECTIONS_PER_VERTEX 4

#define MAXVERTICES MAX_SHAPES * MAX_VERTICES_PER_SHAPE

#define SCREENWIDTH 3000
#define SCREENHEIGHT 2000

// Field of View in degrees
#define fov 90.0


#define UNUSED(x) (void)x

#define CHK(x) if (x) { printf("ERROR.\n"); }


struct location
{
    float x;
    float y;
    float z;
};

struct axis
{
    char x;
    char y;
    char z;
};

struct rgb
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct shape
{
    // Stores the homogeneous coordinates of each vertex.
    float vertices[MAX_VERTICES_PER_SHAPE * NUMBER_OF_HOMOGENEOUS_COORDS];
    // Number of currently occupied vertex spots for the shape.
    int numVertices;
    // Each shape needs a normal vector of magnitude 1.
    float normal[NUMBER_OF_HOMOGENEOUS_COORDS];
    // For polygon filling.
    float maxx, minx, maxy, miny;
    // Connecting vertices together
    int connections[MAX_VERTICES_PER_SHAPE];
    // Color of the shape, RGBA???.
    struct rgb color;
};