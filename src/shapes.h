#ifndef SHAPES_H
#define SHAPES_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

struct Shape{
    vec3** vertices;
    vec4** colors;
    Shape** shapes;
    int numShapes;

    virtual void applyMatrix(mat3x3);
    virtual GLfloat* toVertexArray();
    virtual GLfloat* toColorArray();
    virtual int numPoints();

    virtual int numVertices();
    virtual int numColors();
    virtual ~Shape();
};

struct Triangle: public Shape{
    Triangle(vec3, vec3, vec3, vec4);
    int numVertices();
    int numColors();
    int numPoints();
};

struct Rectangle: public Shape{
    Rectangle(vec3 ul, vec3 ur, vec3 ll, vec3 lr, vec4 colour);
};

struct Box: public Shape{
    Box(vec3 center, double h, double w, double l, vec4 colour);
};

struct Cuboid: public Shape{
    Cuboid(vec3 center1, vec3 center2, double width1, double width2, double height1, double height2, vec4 color);
};

#endif /*SHAPES_H*/