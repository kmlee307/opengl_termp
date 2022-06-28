
#ifndef SPHERE_H
#define SPHERE_H

#include "shader.h"
#include <iostream>
#include <vector>

using namespace std;

bool  isInited = false;

int lats = 30;
int longs = 30;
int i, j;

//#define PI 3.141592654
#define TWOPI 6.283185308
#define radius 1.0

class Sphere {
public:
    vector<GLfloat> vertices;
    vector<GLfloat> textures;
    vector<GLuint> indices;
    vector<GLfloat> normals;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    Sphere() {
        computation();
        initBuffers();

    }
    unsigned int vSize = sizeof(vertices);
    unsigned int tSize = sizeof(textures);
    unsigned int iSize = sizeof(indices);
    unsigned int nSize = sizeof(normals);

    void initBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat) + textures.size() * sizeof(GLfloat)+normals.size()*sizeof(GLfloat), 0, GL_STATIC_DRAW);// copy verts at offset 0
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), &vertices[0]);         // copy verts at offset 0
        glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), textures.size() * sizeof(GLfloat), &textures[0]);         // copy verts at offset 0
        glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat)+textures.size() * sizeof(GLfloat), normals.size() * sizeof(GLfloat), &normals[0]);         // copy verts at offset 0

        // copy index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)(vertices.size() * sizeof(GLfloat)));  // position attrib
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)(vertices.size() * sizeof(GLfloat) + textures.size() * sizeof(GLfloat)));  // position attrib
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };

    void computation() {
        const float PI = acos(-1);

        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
        float s, t;                                     // texCoord

        float sectorStep = 2 *PI / longs;
        float stackStep = PI / lats;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= lats; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= longs; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // normalized vertex normal
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);

                // vertex tex coord between [0, 1]
                s = (float)j / longs;
                t = (float)i / lats;
                textures.push_back(s);
                textures.push_back(t);
            }
        }

        // indices
        //  k1--k1+1
        //  |  / |
        //  | /  |
        //  k2--k2+1
        unsigned int k1, k2;
        for (int i = 0; i < lats; ++i)
        {
            k1 = i * (lats + 1);     // beginning of current stack
            k2 = k1 + lats+ 1;      // beginning of next stack

            for (int j = 0; j < longs; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding 1st and last stacks
                if (i != 0)
                {
                    indices.push_back(k1);   // k1---k2---k1+1
                    indices.push_back(k2);   // k1---k2---k1+1
                    indices.push_back(k1+1);   // k1---k2---k1+1
                }

                if (i != (lats - 1))
                {
                    indices.push_back(k1 + 1); // k1+1---k2---k2+1
                    indices.push_back(k2); // k1+1---k2---k2+1
                    indices.push_back(k2 + 1); // k1+1---k2---k2+1
                }

            }
        }
        
    }

    void deletebuffer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }


    void draw(Shader* shader) {
        shader->use();
        glBindVertexArray(VAO);
        //glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
        glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };

};


#endif
