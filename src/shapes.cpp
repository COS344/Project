#include "shapes.h"
#include <glm/fwd.hpp>

int Shape::numPoints()
{
    int count = 0;
    for (int i = 0; i < numShapes; i++)
    {
        count += shapes[i]->numPoints();
    }
    return count;
}

GLfloat *Shape::toVertexArray()
{
    int n = numVertices();
    GLfloat *result = new GLfloat[numVertices()];
    if (numShapes > 0)
    {
        int count = 0;
        for (int i = 0; i < numShapes; i++)
        {
            GLfloat *temp = shapes[i]->toVertexArray();
            for (int j = 0; j < shapes[i]->numVertices(); j++)
            {
                result[count++] = temp[j];
            }
            delete[] temp;
        }
    }
    else
    {
        int count = 0;
        for (int i = 0; i < numPoints(); i++)
        {
            result[count++] = (*vertices[i])[0];
            result[count++] = (*vertices[i])[1];
            result[count++] = (*vertices[i])[2];
        }
    }

    return result;
}

GLfloat *Shape::toColorArray()
{
    GLfloat *result = new GLfloat[numColors()];
    if (numShapes > 0)
    {
        int count = 0;
        for (int i = 0; i < numShapes; i++)
        {
            GLfloat *temp = shapes[i]->toColorArray();
            for (int j = 0; j < shapes[i]->numColors(); j++)
            {
                result[count++] = temp[j];
            }
            delete[] temp;
        }
    }
    else
    {
        int count = 0;
        for (int i = 0; i < numPoints(); i++)
        {
            result[count++] = (*colors[i])[0];
            result[count++] = (*colors[i])[1];
            result[count++] = (*colors[i])[2];
        }
    }
    return result;
}

void Shape::applyMatrix(mat3x3 m)
{
    if (numShapes > 0)
    {
        for (int i = 0; i < numShapes; i++)
        {
            shapes[i]->applyMatrix(m);
        }
    }
    else
    {
        for (int i = 0; i < numPoints(); i++)
        {
            vec4 point = vec4((*vertices[i])[0], (*vertices[i])[1],(*vertices[i])[2], 1);
            vec3 result = m * point;
            (*vertices[i])[0] = result[0];
            (*vertices[i])[1] = result[1];
            (*vertices[i])[2] = result[2];
        }
    }
}

int Shape::numVertices()
{
    int count = 0;
    for (int i = 0; i < numShapes; i++)
    {
        count += shapes[i]->numVertices();
    }
    return count;
}

int Shape::numColors()
{
    int count = 0;
    for (int i = 0; i < numShapes; i++)
    {
        count += shapes[i]->numColors();
    }
    return count;
}

Shape::~Shape()
{
    if(numShapes <= 0){
        for (int i = 0; i < numPoints(); i++)
        {
            delete vertices[i];
        }
        delete[] vertices;
        for (int i = 0; i < numPoints(); i++)
        {
            delete colors[i];
        }
        delete[] colors;
    }
    else{
        for (int i = 0; i < numShapes; i++)
        {
            delete shapes[i];
        }
        delete[] shapes;
    }
}

Triangle::Triangle(vec3 point1, vec3 point2, vec3 point3, vec4 colour)
{
    numShapes = 0;
    shapes = new Shape *[0];

    int n = 3;
    vertices = new vec3 *[n];
    vertices[0] = new vec3(3);
    (*vertices[0])[0] = point1.x;
    (*vertices[0])[1] = point1.y;
    (*vertices[0])[2] = point1.z;
    vertices[1] = new vec3(3);
    (*vertices[1])[0] = point2.x;
    (*vertices[1])[1] = point2.y;
    (*vertices[1])[2] = point2.z;
    vertices[2] = new vec3(3);
    (*vertices[2])[0] = point3.x;
    (*vertices[2])[1] = point3.y;
    (*vertices[2])[2] = point3.z;

    colors = new vec4*[n+1];
    for (int i = 0; i < 4; i++)
    {
        colors[i] = new vec4(4);
        (*colors[i])[0] = colour.x;
        (*colors[i])[1] = colour.y;
        (*colors[i])[2] = colour.z;
        (*colors[i])[3] = colour.w;
    }
}

int Triangle::numVertices()
{
    return 9;
}

int Triangle::numColors()
{
    return 9;
}

int Triangle::numPoints()
{
    return 3;
}

Rectangle::Rectangle(vec3 ul, vec3 ur, vec3 ll, vec3 lr, vec4 colour)
{
    numShapes = 2;
    shapes = new Shape *[numShapes];
    shapes[0] = new Triangle(ur, ul, ll, colour);
    shapes[1] = new Triangle(ll, lr, ur, colour);
    int n = numPoints();
    vertices = NULL;

    colors = NULL;
}

Box::Box(vec3 center, double height, double width, double length, vec4 colour)
{
    vec3 topPlane = center + vec3(0.0, height / 2.0, 0.0);
    vec3 bottomPlane = center - vec3(0.0, height / 2.0, 0.0);
    vec3 leftPlane = center - vec3(width / 2.0, 0.0, 0.0);
    vec3 rightPlane = center + vec3(width / 2.0, 0.0, 0.0);
    vec3 nearPlane = center - vec3(0.0, 0.0, length / 2.0);
    vec3 farPlane = center + vec3(0.0, 0.0, length / 2.0);

    vec3 nearLeftTop = nearPlane + leftPlane + topPlane;
    vec3 nearRightTop = nearPlane + rightPlane + topPlane;
    vec3 nearLeftBottom = nearPlane + leftPlane + bottomPlane;
    vec3 nearRightBottom = nearPlane + rightPlane + bottomPlane;

    vec3 farLeftTop = farPlane + leftPlane + topPlane;
    vec3 farRightTop = farPlane + rightPlane + topPlane;
    vec3 farLeftBottom = farPlane + leftPlane + bottomPlane;
    vec3 farRightBottom = farPlane + rightPlane + bottomPlane;
                                                                                        //Not sure about these vec4's...
    Rectangle *front = new Rectangle(nearLeftTop, nearRightTop, nearLeftBottom, nearRightBottom, colour*vec4(0.1));
    Rectangle *back = new Rectangle(farRightTop, farLeftTop, farRightBottom, farLeftBottom, colour*vec4(0.2));
    Rectangle *left = new Rectangle(farLeftTop, nearLeftTop, farLeftBottom, nearLeftBottom, colour*vec4(0.3));
    Rectangle *right = new Rectangle(nearRightTop, farRightTop, nearRightBottom, farRightBottom, colour*vec4(0.4));
    Rectangle *bottom = new Rectangle(nearLeftBottom, nearRightBottom, farLeftBottom, farRightBottom, colour*vec4(0.5));
    Rectangle *top = new Rectangle(nearLeftTop, nearRightTop, farLeftTop, farRightTop, colour*vec4(0.6));

    numShapes = 6;
    shapes = new Shape *[numShapes]
    {
        front, back, left, right, bottom, top
    };
}

Cuboid::Cuboid(vec3 center1, vec3 center2, double width1, double width2, double height1, double height2, vec4 colour){
    //Two rectangles, at center1 and center2, with width1 and height1 being for center1, and width2 and height2 being for center2
    Rectangle *rect1 = new Rectangle(center1 + vec3(width1 / 2.0, height1 / 2.0, 0.0), center1 + vec3(-width1 / 2.0, height1 / 2.0, 0.0), center1 + vec3(width1 / 2.0, -height1 / 2.0, 0.0), center1 + vec3(-width1 / 2.0, -height1 / 2.0, 0.0), colour);
    Rectangle *rect2 = new Rectangle(center2 + vec3(width2 / 2.0, height2 / 2.0, 0), center2 + vec3(-width2 / 2.0, height2 / 2.0, 0.0), center2 + vec3(width2 / 2.0, -height2 / 2.0, 0.0), center2 + vec3(-width2 / 2.0, -height2 / 2.0, 0.0), colour);
    
    //Connect the two rectangles with 4 rectangles
    Rectangle *rect3 = new Rectangle(center1 + vec3(width1 / 2.0, height1 / 2.0, 0.0), center2 + vec3(width2 / 2.0, height2 / 2.0, 0.0), center1 + vec3(width1 / 2.0, -height1 / 2.0, 0.0), center2 + vec3(width2 / 2.0, -height2 / 2.0, 0.0), colour);
    Rectangle *rect4 = new Rectangle(center1 + vec3(-width1 / 2.0, height1 / 2.0, 0.0), center2 + vec3(-width2 / 2.0, height2 / 2.0, 0.0), center1 + vec3(-width1 / 2.0, -height1 / 2.0, 0.0), center2 + vec3(-width2 / 2.0, -height2 / 2.0, 0.0), colour);
    Rectangle *rect5 = new Rectangle(center1 + vec3(width1 / 2.0, height1 / 2.0, 0.0), center2 + vec3(width2 / 2.0, height2 / 2.0, 0.0), center1 + vec3(-width1 / 2.0, height1 / 2.0, 0.0), center2 + vec3(-width2 / 2.0, height2 / 2.0, 0.0), colour);
    Rectangle *rect6 = new Rectangle(center1 + vec3(width1 / 2.0, -height1 / 2.0, 0.0), center2 + vec3(width2 / 2.0, -height2 / 2.0, 0.0), center1 + vec3(-width1 / 2.0, -height1 / 2.0, 0.0), center2 + vec3(-width2 / 2.0, -height2 / 2.0, 0.0), colour);

    numShapes = 6;
    shapes = new Shape *[numShapes]
    {
        rect1, rect2, rect3, rect4, rect5, rect6
    };
}

Building::Building() {
    cout << "Building created! :)" << endl;
}

Building::~Building() {
    cout << "Building deleted! :(" << endl;
}

void Building::applyRotation() {

}

void Building::applyTranslation() {

}