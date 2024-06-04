#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "shapes.h"

#define timeDT std::chrono::_V2::steady_clock::time_point;

using namespace std;
using namespace std;

const char *getError()
{
    const char *errorDescription;
    glfwGetError(&errorDescription);
    return errorDescription;
}

inline void startUpGLFW()
{
    glewExperimental = true; 
    if (!glfwInit())
    {
        throw getError();
    }
}

inline void startUpGLEW()
{
    glewExperimental = true; 
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw getError();
    }
}

inline GLFWwindow *setUp()
{
    startUpGLFW();
    glfwWindowHint(GLFW_SAMPLES, 4);               
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    GLFWwindow *window;                                            
    window = glfwCreateWindow(1000, 1000, "Byte Brigade IT café render", NULL, NULL);
    if (window == NULL)
    {
        cout << getError() << endl;
        glfwTerminate();
        throw "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
    }
    glfwMakeContextCurrent(window); 
    startUpGLEW();
    return window;
}

int main() {
    cout << "Hello :)" << endl;

    GLFWwindow *window;
    try
    {
        window = setUp();
    }
    catch (const char *e)
    {
        cout << e << endl;
        throw;
    }

    glClearColor(0, 0.2, 0.13, 1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_NEAREST);
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    GLuint programID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);

    double lastTime;
    lastTime = glfwGetTime();
    bool isWireframe = false;
    auto lastToggleTime = std::chrono::high_resolution_clock::now();

    Shape* building = new Building();
    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        // Here we obtain the vertices and colors for the house as two dynamic arrays.
        GLfloat *vertices;
        GLfloat *colors;

            vertices = building->toVertexArray();
            colors = building->toColorArray();

            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat[building->numVertices()]), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat[building->numColors()]), colors, GL_STATIC_DRAW);
        
        //  Here we bind the VBOs

        // Here we enable the VAO and populate it.
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,        // location 0 in the vertex shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,        // location 1 in the vertex shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

            glDrawArrays(GL_TRIANGLES, 0, building->numPoints()); // Starting from vertex 0; 3 vertices total -> 1 triangle

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Here we swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        //Translation to the forwards
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            // building->applyTranslation();
        }

        //Translation backwards
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            // building->applyTranslation();
        }

        //Translation to the left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            // building->applyTranslation();
        }

        //Translation to the right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            // building->applyTranslation();
        }

        //Rotation to the right
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            // building->applyRotation();
        }

        //Rotation to the left
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            // building->applyRotation();
        }

        //Rotation upwards
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        {
            // building->applyRotation();
        }

        //Rotation downwards
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            // building->applyRotation();
        }
    
        delete[] vertices;
        delete[] colors;

    } while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);
    
    delete building;
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteProgram(programID);
    glfwTerminate();

    return 0;
}