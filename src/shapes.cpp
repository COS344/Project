
#include "shapes.h"

Shape::~Shape()
{
}

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
            result[count++] = colors[i][0];
            result[count++] = colors[i][1];
            result[count++] = colors[i][2];
        }
    }
    return result;
}

void Shape::applyMatrix(mat4x4 m)
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
            (*vertices[i]) = m * vec4((*vertices[i]), 1);
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

Triangle::Triangle(vec3 point1, vec3 point2, vec3 point3, vec3 color)
{
    numShapes = 0;
    shapes = new Shape *[0];

    int n = numPoints();
    vertices = new vec3 *[n];
    vertices[0] = new vec3(point1);
    vertices[1] = new vec3(point2);
    vertices[2] = new vec3(point3);

    colors = new vec3[n];
    for (int i = 0; i < 3; i++)
    {
        colors[i] = color;
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

Rectangle::Rectangle(vec3 ul, vec3 ur, vec3 ll, vec3 lr, vec3 color)
{
    numShapes = 2;
    shapes = new Shape *[numShapes];
    shapes[0] = new Triangle(ur, ul, ll, color);
    shapes[1] = new Triangle(ll, lr, ur, color);
    int n = numPoints();
    vertices = new vec3 *[n];
    int count = 0;
    for (int i = 0; i < numShapes; i++)
    {
        for (int j = 0; j < shapes[i]->numPoints(); j++)
        {
            vertices[count++] = shapes[i]->vertices[j];
        }
    }

    colors = new vec3[n];

    for (int i = 0; i < n; i++)
    {
        colors[i] = color;
    }
}

Box::Box(vec3 center, double height, double width, double length, vec3 color)
{
    vec3 topPlane = center + vec3(0, height / 2, 0);
    vec3 bottomPlane = center - vec3(0, height / 2, 0);
    vec3 leftPlane = center - vec3(width / 2, 0, 0);
    vec3 rightPlane = center + vec3(width / 2, 0, 0);
    vec3 nearPlane = center - vec3(0, 0, length / 2);
    vec3 farPlane = center + vec3(0, 0, length / 2);

    vec3 nearLeftTop = nearPlane + leftPlane + topPlane;
    vec3 nearRightTop = nearPlane + rightPlane + topPlane;
    vec3 nearLeftBottom = nearPlane + leftPlane + bottomPlane;
    vec3 nearRightBottom = nearPlane + rightPlane + bottomPlane;

    vec3 farLeftTop = farPlane + leftPlane + topPlane;
    vec3 farRightTop = farPlane + rightPlane + topPlane;
    vec3 farLeftBottom = farPlane + leftPlane + bottomPlane;
    vec3 farRightBottom = farPlane + rightPlane + bottomPlane;

    Rectangle *front = new Rectangle(nearLeftTop, nearRightTop, nearLeftBottom, nearRightBottom, color*0.1f);
    Rectangle *back = new Rectangle(farRightTop, farLeftTop, farRightBottom, farLeftBottom, color*0.2f);
    Rectangle *left = new Rectangle(farLeftTop, nearLeftTop, farLeftBottom, nearLeftBottom, color*0.3f);
    Rectangle *right = new Rectangle(nearRightTop, farRightTop, nearRightBottom, farRightBottom, color*0.4f);
    Rectangle *bottom = new Rectangle(nearLeftBottom, nearRightBottom, farLeftBottom, farRightBottom, color*0.5f);
    Rectangle *top = new Rectangle(nearLeftTop, nearRightTop, farLeftTop, farRightTop, color*0.6f);

    numShapes = 6;
    shapes = new Shape *[numShapes]
    {
        front, back, left, right, bottom, top
    };
}

Boxes::Boxes(int numBoxes, vec3 *centers, double *heights, double *widths, double *lengths, vec3 *colors)
{
    numShapes = numBoxes;
    shapes = new Shape*[numShapes];
    for(int i=0; i < numShapes; i++){
        shapes[i] = new Box(centers[i],heights[i], widths[i], lengths[i], colors[i]);
    }
}

Building::Building(){
    
    numShapes = 3;
    shapes = new Shape *[numShapes];

    // East Walls
    vec3 centers[3] = {
        vec3(0, 0, 0),
        vec3(0.04, 0, 0.18),
        vec3(0.04, 0, -0.18)};
    double heights[3] = {
        0.5, // mid wall height
        0.5, // left wall front side
        0.5 // right wall front side
    };
    double widths[3] = {
        0.5,
        0.25,
        0.25,
    };
    double lengths[3] = {
        0.6,
        0.15,
        0.15,
    };
    vec3 colors[3] = {
        vec3(0, 0, 1),
        vec3(1, 0, 0),
        vec3(0, 1, 0)};

    shapes[0] = new Boxes(3, centers, heights, widths, lengths, colors);

    // Left front side windows and right side
    vec3 centersw[20] = {
        vec3(0.04, 0.065, 0.14),
        vec3(0.04, 0.04, 0.14),
        vec3(0.04, 0.015, 0.14),
        vec3(0.04, -0.01, 0.14),
        vec3(0.04, -0.03, 0.14),
        vec3(0.04, 0.065, 0.115),
        vec3(0.04, 0.04, 0.115),
        vec3(0.04, 0.015, 0.115),
        vec3(0.04, -0.01, 0.115),
        vec3(0.04, -0.03, 0.115),

        //right side
        vec3(0.04, 0.065, -0.14),
        vec3(0.04, 0.04, -0.14),
        vec3(0.04, 0.015, -0.14),
        vec3(0.04, -0.01, -0.14),
        vec3(0.04, -0.03, -0.14),
        vec3(0.04, 0.065, -0.115),
        vec3(0.04, 0.04, -0.115),
        vec3(0.04, 0.015, -0.115),
        vec3(0.04, -0.01, -0.115),
        vec3(0.04, -0.03, -0.115)};

    double heightsw[20] = {
        0.10, 
        0.05, 
        0.10,
        0.05,
        0.08,
        0.10, 
        0.05, 
        0.10,
        0.05,
        0.08,

        //right side
        0.10, 
        0.05, 
        0.10,
        0.05,
        0.08,
        0.10, 
        0.05, 
        0.10,
        0.05,
        0.08
    };
    double widthsw[20] = {
        0.01,
        0.01,
        0.01,
        0.01,
        0.01,
        0.01,
        0.01,
        0.01,
        0.01,
        0.01,

        //right side

        0.01,
        0.01,
        0.01,
        0.01,
        0.01,
        0.01,
        0.01,
        0.01,
        0.01,
        0.01
    };
    double lengthsw[20] = {
        0.08,
        0.08,
        0.08,
        0.08,
        0.08,
        0.08,
        0.08,
        0.08,
        0.08,
        0.08,

        //right side
        0.08,
        0.08,
        0.08,
        0.08,
        0.08,
        0.08,
        0.08,
        0.08,
        0.08,
        0.08
    };
    vec3 colorsw[20] = {
        vec3(0, 1, 0),
        vec3(0, 1, 1),
        vec3(0, 1, 0),
        vec3(0, 1, 1),
        vec3(0, 0, 0),
        vec3(0, 1, 0),
        vec3(0, 1, 1),
        vec3(0, 1, 0),
        vec3(0, 1, 1),
        vec3(0, 0, 0),
        
        //right side
        vec3(1, 1, 0),
        vec3(1, 1, 1),
        vec3(1, 1, 0),
        vec3(1, 1, 1),
        vec3(0, 0, 0),
        vec3(0, 1, 0),
        vec3(0, 1, 1),
        vec3(0, 1, 0),
        vec3(0, 1, 1),
        vec3(0, 0, 0)};

    shapes[1] = new Boxes(20, centersw, heightsw, widthsw, lengthsw, colorsw);



    // Doors left and right side
    vec3 centerD[4] = {
        vec3(0.04, -0.064, 0.14),
        vec3(0.04, -0.064, 0.115),
        vec3(0.04, -0.064, -0.14),
        vec3(0.04, -0.064, -0.115)};
    double heightD[4] = {
        0.12, 
        0.12, 
        0.12,
        0.12
    };
    double widthD[4] = {
        0.01,
        0.01,
        0.01,
        0.01
    };
    double lengthD[4] = {
        0.08,
        0.08,
        0.08,
        0.08
    };
    vec3 colorD[4] = {
        vec3(1, 0.5, 0.5),
        vec3(1, 1, 1),
        vec3(1, 0.5, 0.5),
        vec3(1, 1, 1)};

    shapes[2] = new Boxes(4, centerD, heightD, widthD, lengthD, colorD);

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