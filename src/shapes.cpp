
#include "shapes.h"
#include <glm/trigonometric.hpp>

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

Prisma::Prisma(vec3 center, double height, double width, double length, vec3 color)
{
    vec3 topPlane = center + vec3( 0.0,  height / 2.0, 0.0);
    vec3 bottomPlane = center - vec3( 0,  height / 2, 0);
    vec3 leftPlane = center - vec3(width / 2, 0, 0);
    vec3 rightPlane = center + vec3(width / 2, 0, 0);
    vec3 nearPlane = center - vec3( 0, 0, length / 2);
    vec3 farPlane = center + vec3( 0, 0, length / 2);

    vec3 nearLeftTop = nearPlane + leftPlane + topPlane;
    vec3 nearRightTop = nearPlane + rightPlane + topPlane;
    vec3 nearLeftBottom = nearPlane + leftPlane + bottomPlane;
    vec3 nearRightBottom = nearPlane + rightPlane + bottomPlane;

    vec3 farLeftTop = farPlane + leftPlane + topPlane;
    vec3 farRightTop = farPlane + rightPlane + topPlane;
    vec3 farLeftBottom = farPlane + leftPlane + bottomPlane;
    vec3 farRightBottom = farPlane + rightPlane + bottomPlane;

    vec3 topPlaneFrontCenter = nearLeftTop;
    vec3 topPlaneBackCenter = farLeftTop;


    Triangle *front = new Triangle(topPlaneFrontCenter, nearLeftBottom, nearRightBottom, color);
    Triangle *back = new Triangle(topPlaneBackCenter, farLeftBottom, farRightBottom, color);
    Rectangle *left = new Rectangle(topPlaneBackCenter, topPlaneFrontCenter, farLeftBottom, nearLeftBottom, color);
    Rectangle *right = new Rectangle(topPlaneFrontCenter, topPlaneBackCenter, nearRightBottom, farRightBottom, color);
    Rectangle *bottom = new Rectangle(nearLeftBottom, nearRightBottom, farLeftBottom, farRightBottom, color);

    numShapes = 5;
    shapes = new Shape *[numShapes]
    {
        bottom, front, back, left, right
    };
}

Radar1::Radar1(vec3 center, double height, double width, double length, vec3 color)
{
    vec3 topPlane = center + vec3(0.0,  height / 2.0, 0.0);
    vec3 bottomPlane = center - vec3( 0.0,  height / 2.0, 0.0);
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

    vec3 topPlaneCenter = farLeftTop;

    Triangle *front = new Triangle(topPlaneCenter, nearLeftBottom, nearRightBottom, color);
    Triangle *back = new Triangle(topPlaneCenter, farLeftBottom, farRightBottom, color);
    Triangle *left = new Triangle(topPlaneCenter, farLeftBottom, nearLeftBottom, color);
    Triangle *right = new Triangle(topPlaneCenter, nearRightBottom, farRightBottom, color);
    Rectangle *bottom = new Rectangle(nearLeftBottom, nearRightBottom, farLeftBottom, farRightBottom, color);

    numShapes = 5;
    shapes = new Shape *[numShapes]
    {
        bottom, front, back, left, right
    };
}

Building::Building(){
    
    numShapes = 14;
    shapes = new Shape *[numShapes];
    int scalingFactor = 10;

    // East Walls
    vec3 centers[4] = {
        vec3(0, 0, 0),
        vec3(0.04, 0, 0.18),
        vec3(0.04, 0, -0.18),
        vec3(-0.875, 0.0, 0.0)};

    for (int i = 0; i < 4; i++) {
        centers[i].x *= scalingFactor;
        centers[i].y *= scalingFactor;
        centers[i].z *= scalingFactor;
    }

    double heights[4] = {
        0.5 * scalingFactor, // mid wall height
        0.5 * scalingFactor, // left wall front side
        0.5 * scalingFactor, // right wall front side
        0.5 * scalingFactor
    };

    double widths[4] = {
        0.5 * scalingFactor,
        0.25 * scalingFactor,
        0.25 * scalingFactor,
        0.25 * scalingFactor,
    };

    double lengths[4] = {
        0.6 * scalingFactor,
        0.15 * scalingFactor,
        0.15 * scalingFactor,
        1.3 * scalingFactor,
    };

    vec3 colors[4] = {
        vec3(0.5, 0.5, 0.5),
        vec3(0.5, 0.5, 0.5),
        vec3(0.5, 0.5, 0.5),
        vec3(0.5, 0.5, 0.5),
        };

    shapes[0] = new Boxes(4, centers, heights, widths, lengths, colors);

    // Left front side windows and right side

    vec3 centersw[20] = {
        //left side
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

    for (int i = 0; i < 20; i++) {
        centersw[i].x *= scalingFactor;
        centersw[i].y *= scalingFactor;
        centersw[i].z *= scalingFactor;
    }

    double heightsw[20] = {
        0.10 * scalingFactor, 
        0.05 * scalingFactor, 
        0.10 * scalingFactor,
        0.05 * scalingFactor,
        0.08 * scalingFactor,
        0.10 * scalingFactor, 
        0.05 * scalingFactor, 
        0.10 * scalingFactor,
        0.05 * scalingFactor,
        0.08 * scalingFactor,

        //right side
        0.10 * scalingFactor, 
        0.05 * scalingFactor, 
        0.10 * scalingFactor,
        0.05 * scalingFactor,
        0.08 * scalingFactor,
        0.10 * scalingFactor, 
        0.05 * scalingFactor, 
        0.10 * scalingFactor,
        0.05 * scalingFactor,
        0.08 * scalingFactor
    };
    
    double widthsw[20] = {
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,

        //right side

        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor
    };

    double lengthsw[20] = {
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,

        //right side
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor
    };

    //Windoewwwws
    vec3 colorsw[20] = {
        //left side
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        
        
        //right side
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0),
        vec3(0.75, 0.75, 1.0)};

    shapes[1] = new Boxes(20, centersw, heightsw, widthsw, lengthsw, colorsw);



    // Doors left and right side
    vec3 centerD[4] = {
        vec3(0.04, -0.064, 0.14),
        vec3(0.04, -0.064, 0.115),
        vec3(0.04, -0.064, -0.14),
        vec3(0.04, -0.064, -0.115)};

    for (int i = 0; i < 4; i++) {
        centerD[i].x *= scalingFactor;
        centerD[i].y *= scalingFactor;
        centerD[i].z *= scalingFactor;
    }

    double heightD[4] = {
        0.12 * scalingFactor, 
        0.12 * scalingFactor, 
        0.12 * scalingFactor,
        0.12 * scalingFactor
    };
    double widthD[4] = {
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor,
        0.01 * scalingFactor
    };
    double lengthD[4] = {
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor,
        0.08 * scalingFactor
    };

    vec3 colorD[4] = {
        vec3(0.9, 0.9, 0.9),
        vec3(0.9, 0.9, 0.9),
        vec3(0.9, 0.9, 0.9),
        vec3(1.0, 1.0, 1.0)
    };

    shapes[2] = new Boxes(4, centerD, heightD, widthD, lengthD, colorD);

    //Floor :)
    vec3 fcenters[1] = {
        vec3(-4.0, -0.853, 0)
    };

    double fheights[1] = {
        0.1 
    };

    double fwidths[1] = {
        3.0 * scalingFactor
    };

    double flengths[1] = {
        1.3 * scalingFactor
    };

    vec3 fcolors[1] = {
        vec3(0.2, 0.2, 0.2),
    };

    shapes[3] = new Boxes(1, fcenters, fheights, fwidths, flengths, fcolors);

    //Walls - North and south
    vec3 ncenters[2] = {
        vec3(-0.4, 0, -0.221),
        vec3(-0.4, 0, 0.221),
    };


    for (int i = 0; i < 2; i++) {
        ncenters[i].x *= scalingFactor;
        ncenters[i].y *= scalingFactor;
        ncenters[i].z *= scalingFactor;
    }

    double nheights[2] = {
        0.5 * scalingFactor, // mid wall height
        0.5 * scalingFactor, // left wall front side
    };

    double nwidths[2] = {
        2.75 * scalingFactor,
        2.75 * scalingFactor,
    };

    double nlengths[2] = {
        0.1 * scalingFactor,
        0.1 * scalingFactor,
    };

    vec3 ncolors[2] = {
        vec3(0.5, 0.5, 0.5),
        vec3(0.5, 0.5, 0.5),
        };

    shapes[4] = new Boxes(2, ncenters, nheights, nwidths, nlengths, ncolors);
    shapes[5] = new Prisma(vec3(-1.95, 0.5, 4.0), 0.125 * scalingFactor, 0.0625 * scalingFactor, 2.75 * scalingFactor, vec3(0.5, 0.5, 0.5));
    shapes[6] = new Prisma(vec3(-1.95, 0.5, -4.0), 0.125 * scalingFactor, 0.0625 * scalingFactor, 2.75 * scalingFactor, vec3(0.5, 0.5, 0.5));
            
            // Rotation about x-axis in an anti-clockwise direction
            float angle = glm::radians(90.0f);
            mat4x4 rotationY = mat4x4(0.0f);
            rotationY[0].x = cos(angle);
            rotationY[1].y = 1;
            rotationY[0].z = -sin(angle);
            rotationY[2].x = sin(angle);
            rotationY[2].z = cos(angle);
            rotationY[3].w = 1;

            shapes[5]->applyMatrix(transpose(rotationY));

            float angle2 = glm::radians(-90.0f);
            mat4x4 rotationY2 = mat4x4(0.0f);
            rotationY2[0].x = cos(angle2);
            rotationY2[1].y = 1;
            rotationY2[0].z = -sin(angle2);
            rotationY2[2].x = sin(angle2);
            rotationY2[2].z = cos(angle2);
            rotationY2[3].w = 1;

            shapes[6]->applyMatrix(transpose(rotationY2));

    shapes[7] = new Prisma(vec3(-1.95, -0.25, 4.0), 0.125 * scalingFactor, 0.0625 * scalingFactor, 2.75 * scalingFactor, vec3(0.5, 0.5, 0.5));
    shapes[8] = new Prisma(vec3(-1.95, -0.25, -4.0), 0.125 * scalingFactor, 0.0625 * scalingFactor, 2.75 * scalingFactor, vec3(0.5, 0.5, 0.5));
            
            // Rotation about x-axis in an anti-clockwise direction
            float angle3 = glm::radians(90.0f);
            mat4x4 rotationY3 = mat4x4(0.0f);
            rotationY3[0].x = cos(angle3);
            rotationY3[1].y = 1;
            rotationY3[0].z = -sin(angle3);
            rotationY3[2].x = sin(angle3);
            rotationY3[2].z = cos(angle3);
            rotationY3[3].w = 1;

            shapes[7]->applyMatrix(transpose(rotationY3));

            float angle4 = glm::radians(-90.0f);
            mat4x4 rotationY4 = mat4x4(0.0f);
            rotationY4[0].x = cos(angle4);
            rotationY4[1].y = 1;
            rotationY4[0].z = -sin(angle4);
            rotationY4[2].x = sin(angle4);
            rotationY4[2].z = cos(angle4);
            rotationY4[3].w = 1;

            shapes[8]->applyMatrix(transpose(rotationY4));

    //Box
    shapes[9] = new Box(vec3(-3.75, 1.2, 0.0), 0.01 * scalingFactor, 3 * scalingFactor, 0.3 * scalingFactor, vec3(0.9, 0.9, 0.9));
    shapes[10] = new Box(vec3(-3.75, 1.15, 0.0), 0.01 * scalingFactor, 3 * scalingFactor, 0.3 * scalingFactor, vec3(0.9, 0.9, 0.9));
    shapes[11] = new Box(vec3(-3.75, 1.00, 0.0), 0.01 * scalingFactor, 3 * scalingFactor, 0.3 * scalingFactor, vec3(0.9, 0.9, 0.9));
    shapes[12] = new Box(vec3(-3.75, 1.0, 0.0), 0.01 * scalingFactor, 3 * scalingFactor, 0.3 * scalingFactor, vec3(0.9, 0.9, 0.9));
    shapes[13] = new Box(vec3(-3.75, 1.15, 0.0), 0.01 * scalingFactor, 3 * scalingFactor, 0.3 * scalingFactor, vec3(0.9, 0.9, 0.9));

    float angle5 = glm::radians(10.0f);
    mat4x4 rotationX = mat4x4(0.0f);
    rotationX[0].x = 1;
    rotationX[1].y = cos(angle5);
    rotationX[1].z = -sin(angle5);
    rotationX[2].y = sin(angle5);
    rotationX[2].z = cos(angle5);
    rotationX[3].w = 1;

    shapes[10]->applyMatrix(transpose(rotationX));
    // shapes[11]->applyMatrix(transpose());
    // shapes[12]->applyMatrix(transpose());

    float angle6 = glm::radians(10.0f);
    mat4x4 rotationX2 = mat4x4(0.0f);
    rotationX2[0].x = 1;
    rotationX2[1].y = cos(-angle6);
    rotationX2[1].z = -sin(-angle6);
    rotationX2[2].y = sin(-angle6);
    rotationX2[2].z = cos(-angle6);
    rotationX2[3].w = 1;
    shapes[13]->applyMatrix(transpose(rotationX2));

    mat4x4 translationZ = mat4x4(0.0f);
    translationZ[0].x = 1;
    translationZ[1].y = 1;
    translationZ[2].z = 1;
    translationZ[3].w = 1;
    translationZ[2].w = 2.25f; 
    shapes[10]->applyMatrix(transpose(translationZ));

    // shapes[11]->applyMatrix(transpose());
    // shapes[12]->applyMatrix(transpose());

    mat4x4 translationZ2 = mat4x4(0.0f);
    translationZ2[0].x = 1;
    translationZ2[1].y = 1;
    translationZ2[2].z = 1;
    translationZ2[3].w = 1;
    translationZ2[2].w = -2.25f; 
    shapes[13]->applyMatrix(transpose(translationZ2));

    float angle7 = glm::radians(20.0f);
    mat4x4 rotationX3 = mat4x4(0.0f);
    rotationX3[0].x = 1;
    rotationX3[1].y = cos(angle7);
    rotationX3[1].z = -sin(angle7);
    rotationX3[2].y = sin(angle7);
    rotationX3[2].z = cos(angle7);
    rotationX3[3].w = 1;
    

    shapes[11]->applyMatrix(transpose(rotationX3));

    float angle8 = glm::radians(20.0f);
    mat4x4 rotationX4 = mat4x4(0.0f);
    rotationX4[0].x = 1;
    rotationX4[1].y = cos(-angle8);
    rotationX4[1].z = -sin(-angle8);
    rotationX4[2].y = sin(-angle8);
    rotationX4[2].z = cos(-angle8);
    rotationX4[3].w = 1;

    shapes[12]->applyMatrix(transpose(rotationX4));


    mat4x4 translationZ3 = mat4x4(0.0f);
    translationZ3[0].x = 1;
    translationZ3[1].y = 1;
    translationZ3[2].z = 1;
    translationZ3[3].w = 1;
    translationZ3[2].w = 4.0f; 
    shapes[11]->applyMatrix(transpose(translationZ3));

    mat4x4 translationZ4 = mat4x4(0.0f);
    translationZ4[0].x = 1;
    translationZ4[1].y = 1;
    translationZ4[2].z = 1;
    translationZ4[3].w = 1;
    translationZ4[2].w = -4.0f; 
    shapes[12]->applyMatrix(transpose(translationZ4));
}



Building::~Building() {
    cout << "Building deleted! :(" << endl;
}

void Building::applyRotation() {

}

void Building::applyTranslation() {


}