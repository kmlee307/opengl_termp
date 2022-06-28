/// Quadrangular Pyramid (사각뿔)
//
// 5 faces (front, right, back, left, and bottom)
// 4 faces are triangles (3 vertices): front, right, back, left
// bottom face is a rectangle (4 vertices)
//
// Vertex shader: the location (0: position attrib (vec3),
//                              1: color attrib (vec4))
//
/*
#ifndef QUADPYRA_H
#define QUADPYRA_H

#include "shader.h"
#include <math.h>

class QuadPyra {
public:

    // vertex position array
    GLfloat vertices[48] = { // (4 faces x 3 vertices x 3) + (4 vertices x 3) = 36 + 12 = 48
        .0f, .5f, .0f,   -.5f, -.5f,  .5f,    .5f, -.5f,  .5f,   // front
        .0f, .5f, .0f,    .5f, -.5f,  .5f,    .5f, -.5f, -.5f,   // right
        .0f, .5f, .0f,    .5f, -.5f, -.5f,   -.5f, -.5f, -.5f,   // back
        .0f, .5f, .0f,   -.5f, -.5f, -.5f,   -.5f, -.5f,  .5f,   // left
       -.5f,-.5f, .5f,   -.5f, -.5f, -.5f,    .5f, -.5f, -.5f,  .5f, -.5f, .5f // bottom
    };

    GLfloat normal[48] = { // 48 elements
    .0f, GLfloat(sqrt(3) / 2), .5f,   .0f, GLfloat(sqrt(3) / 2), .5f,    .0f, GLfloat(sqrt(3) / 2), .5f,   // front
    .5f, GLfloat(sqrt(3) / 2), .0f,   .5f, GLfloat(sqrt(3) / 2), .0f,   .5f, GLfloat(sqrt(3) / 2), .0f,   // right
    .0f, GLfloat(sqrt(3) / 2), -.5f,    .0f, GLfloat(sqrt(3) / 2), -.5f,   .0f, GLfloat(sqrt(3) / 2), -.5f,   // back
    -.5f, GLfloat(sqrt(3) / 2), .0f,   -.5f, GLfloat(sqrt(3) / 2), .0f,   -.5f, GLfloat(sqrt(3) / 2), .0f,   // left
    .0f, -1.0f, .0f,   .0f, -1.0f, .0f,    .0f, -1.0f, .0f,  .0f, -1.0f, .0f // bottom
    };

    // color array
    GLfloat colors[64] = { // (4 faces x 3 vertices x 4) + (4 vertices x 4) = 48 + 16 = 64
        1, 0, 0, 1,  1, 0, 0, 1,  1, 0, 0, 1,    // front: red
        0, 1, 0, 1,  0, 1, 0, 1,  0, 1, 0, 1,    // right: green
        0, 0, 1, 1,  0, 0, 1, 1,  0, 0, 1, 1,    // back: blue
        1, 1, 0, 1,  1, 1, 0, 1,  1, 1, 0, 1,    // left: yellow
        1, 0, 1, 1,  1, 0, 1, 1,  1, 0, 1, 1,  1, 0, 1, 1  // bottom: magenta
    };

    GLfloat texture[32] = { // 48 elements
    .51f, .92f,   .34f, .65f,   .66f, .65f,
    .93f, .49f,   .66f, .65f,   .66f, .34f,
    .51f, .07f,   .66f, .34f,   .34f, .34f,
    .08f, .049f,   .34f, .34f,   .34f, .65f,
    .34f, .65f,   .34f, .34f,   .66f, .34f,   .66f, .65f
    };



    // index array for glDrawElements()

    GLuint cubeIndices[18] = { // (6 triangles x 3 indices)
       0, 1, 2,     // front
       3, 4, 5,     // right
       6, 7, 8,     // back
       9, 10, 11,   // left
       12, 13, 15,  // bottom 1
       15, 14, 13   // bottom 2
    };

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int vSize = sizeof(vertices);
    unsigned int nSize = sizeof(normal);
    unsigned int cSize = sizeof(colors);
    unsigned int tSize = sizeof(texture);



    QuadPyra() {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize + cSize + tSize, 0, GL_STATIC_DRAW); // reserve space
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, vertices);         // copy verts at offset 0
        glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, normal);       // copy cols after positions
        glBufferSubData(GL_ARRAY_BUFFER, vSize + nSize, cSize, colors);       // copy cols after positions
        glBufferSubData(GL_ARRAY_BUFFER, vSize + nSize + cSize, tSize, texture);       // copy cols after positions


        // copy index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vSize); //normal attrib
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(vSize + nSize)); //colors attrib
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vSize + nSize + cSize)); //texture attrib


        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };


    void draw(Shader* shader) {
        shader->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };

};



#endif
*/


// Quadrangular Pyramid (사각뿔)
//
// 5 faces (front, right, back, left, and bottom)
// 4 faces are triangles (3 vertices): front, right, back, left
// bottom face is a rectangle (4 vertices)
//
// Vertex shader: the location (0: position attrib (vec3),
//                              1: color attrib (vec4),
//                              2: texture coordinate attrib (vec2),
//                              3: normal attrib (vec3))
//

#ifndef QUADPYRA_H
#define QUADPYRA_H

#include "shader.h"
#include <iostream>
using namespace std;

class QuadPyra {
public:

    // vertex position array
    GLfloat vertices[48] = { // (4 faces x 3 vertices x 3) + (4 vertices x 3) = 36 + 12 = 48
        .0f, .5f, .0f,   -.5f, -.5f,  .5f,    .5f, -.5f,  .5f,   // front
        .0f, .5f, .0f,    .5f, -.5f,  .5f,    .5f, -.5f, -.5f,   // right
        .0f, .5f, .0f,    .5f, -.5f, -.5f,   -.5f, -.5f, -.5f,   // back
        .0f, .5f, .0f,   -.5f, -.5f, -.5f,   -.5f, -.5f,  .5f,   // left
       -.5f,-.5f, .5f,   -.5f, -.5f, -.5f,    .5f, -.5f, -.5f,  .5f, -.5f, .5f // bottom
    };

    // color array
    GLfloat colors[64] = { // (4 faces x 3 vertices x 4) + (4 vertices x 4) = 48 + 16 = 64
        1, 0, 0, 1,  1, 0, 0, 1,  1, 0, 0, 1,    // front: red
        0, 1, 0, 1,  0, 1, 0, 1,  0, 1, 0, 1,    // right: green
        0, 0, 1, 1,  0, 0, 1, 1,  0, 0, 1, 1,    // back: blue
        1, 1, 0, 1,  1, 1, 0, 1,  1, 1, 0, 1,    // left: yellow
        1, 0, 1, 1,  1, 0, 1, 1,  1, 0, 1, 1,  1, 0, 1, 1  // bottom: magenta
    };

    // texture coord array
    GLfloat texCoords[32] = { // (4 faces x 3 vertices x 2) + (4 vertices x 2) = 24 + 8 = 32
        0.500, 0.604,   0.381, 0.420,   0.619, 0.420,   // front
        0.886, 0.346,   0.619, 0.420,   0.619, 0.271,   // right
        0.500, 0.091,   0.619, 0.271,   0.381, 0.217,   // back
        0.116, 0.346,   0.381, 0.271,   0.381, 0.420,   // left
        0.381, 0.420,   0.381, 0.271,   0.619, 0.271,  0.619, 0.420  // bottom
    };

    // normal arrays
    GLfloat normalVertex[48];
    GLfloat normalFace[48];


    // index array for glDrawElements()
    GLuint cubeIndices[18] = { // (6 triangles x 3 indices)
       0, 1, 2,     // front
       3, 4, 5,     // right
       6, 7, 8,     // back
       9, 10, 11,   // left
       12, 13, 15,  // bottom 1
       15, 14, 13   // bottom 2
    };

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int vSize = sizeof(vertices);
    unsigned int cSize = sizeof(colors);
    unsigned int tSize = sizeof(texCoords);
    unsigned int nSize = sizeof(normalVertex);

    bool isSmoothShading;

    QuadPyra() {
        isSmoothShading = false;
        computeNormals();
        initBuffers();
        //updateBuffers();
    }

    void initBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize + cSize + tSize, 0, GL_STATIC_DRAW); // reserve space
        glBufferData(GL_ARRAY_BUFFER, vSize + cSize + tSize + nSize, 0, GL_STATIC_DRAW); // reserve space
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, vertices);         // copy verts at offset 0
        glBufferSubData(GL_ARRAY_BUFFER, vSize, cSize, colors);       // copy cols after positions
        glBufferSubData(GL_ARRAY_BUFFER, vSize + cSize, tSize, texCoords);  // copy texs after colors
        glBufferSubData(GL_ARRAY_BUFFER, vSize + cSize + tSize, nSize, normalFace);  // copy norms after texs

        // copy index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)vSize); //color attrib
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vSize + cSize)); // tex coord
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vSize + cSize + tSize)); // normal attrib

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };

    // computeNormals: compute face normals and vertex normals

    void computeNormals() {

        // face normals
        for (int i = 0; i < 5; i++) {
            float nx, ny, nz;
            int k = i * 9;
            triangleNormal(vertices[k], vertices[k + 1], vertices[k + 2],
                vertices[k + 3], vertices[k + 4], vertices[k + 5],
                vertices[k + 6], vertices[k + 7], vertices[k + 8],
                nx, ny, nz);
            normalFace[k] = normalFace[k + 3] = normalFace[k + 6] = nx;
            normalFace[k + 1] = normalFace[k + 4] = normalFace[k + 7] = ny;
            normalFace[k + 2] = normalFace[k + 5] = normalFace[k + 8] = nz;
        }

        normalFace[45] = normalFace[42];
        normalFace[46] = normalFace[43];
        normalFace[47] = normalFace[44];

        // test printing
        /*
        for (int i = 0; i < 5; i++) {
            if (i == 0) cout << "FRONT\n";
            else if (i == 1) cout << "RIGHT\n";
            else if (i == 2) cout << "BACK\n";
            else if (i == 3) cout << "LEFT\n";
            else if (i == 4) cout << "BOTTOM\n";
            int k = i * 9;
            cout << "(" << normalFace[k] << ", " << normalFace[k+1] << ", "
                << normalFace[k+2] << ")\n";
            cout << "(" << normalFace[k+3] << ", " << normalFace[k+4] << ", "
                << normalFace[k+5] << ")\n";
            cout << "(" << normalFace[k+6] << ", " << normalFace[k+7] << ", "
                << normalFace[k+8] << ")\n";
        }
        cout << "(" << normalFace[45] << ", " << normalFace[46] << ", "
            << normalFace[47] << ")\n";
        */

        // vertex normals
        float front[3], right[3], back[3], left[3], bottom[3];
        for (int i = 0; i < 3; i++) {
            front[i] = normalFace[0 + i];
            right[i] = normalFace[9 + i];
            back[i] = normalFace[18 + i];
            left[i] = normalFace[27 + i];
            bottom[i] = normalFace[36 + i];
        }

        for (int i = 0; i < 3; i++) {

            normalVertex[0 + i] = (front[i] + right[i] + back[i] + left[i]) / 4.0f;   // front top
            normalVertex[3 + i] = (front[i] + left[i] + bottom[i]) / 3.0f;            // front bottom left
            normalVertex[6 + i] = (front[i] + right[i] + bottom[i]) / 3.0f;           // front bottom right

            normalVertex[9 + i] = normalVertex[0 + i];               // right top = front top
            normalVertex[12 + i] = normalVertex[6 + i];              // right bottom front = front bottom right
            normalVertex[15 + i] = (right[i] + back[i] + bottom[i]) / 3.0f; // right bottom far

            normalVertex[18 + i] = normalVertex[0 + i];             // back top = front top
            normalVertex[21 + i] = normalVertex[15 + i];            // back right far = right bottom far
            normalVertex[24 + i] = (back[i] + left[i] + bottom[i]) / 3.0f;  // back left far

            normalVertex[27 + i] = normalVertex[0 + i];            // left top = front top
            normalVertex[30 + i] = normalVertex[24 + i];           // left bottom far = back left far
            normalVertex[33 + i] = normalVertex[3 + i];            // left bottom front = front bottom left

            normalVertex[36 + i] = normalVertex[3 + i];            // bottom left front = front bottom left
            normalVertex[39 + i] = normalVertex[30 + i];           // bottom left far = left bottom far
            normalVertex[42 + i] = normalVertex[15 + i];           // bottom right far = right bottom far
            normalVertex[45 + i] = normalVertex[6 + i];            // bottom right front = front bottom right

        }

    }

    // triangleNormal: compute the normal vector of a triangle with three given vertices

    void triangleNormal(float v1x, float v1y, float v1z,
        float v2x, float v2y, float v2z,
        float v3x, float v3y, float v3z,
        float& nx, float& ny, float& nz) {

        float A[3], B[3];

        A[0] = v3x - v2x; A[1] = v3y - v2y;  A[2] = v3z - v2z;
        B[0] = v1x - v2x; B[1] = v1y - v2y;  B[2] = v1z - v2z;

        nx = A[1] * B[2] - A[2] * B[1];
        ny = A[2] * B[0] - A[0] * B[2];
        nz = A[0] * B[1] - A[1] * B[0];

        float size = sqrt(nx * nx + ny * ny + nz * nz);
        nx = nx / size;
        ny = ny / size;
        nz = nz / size;
    }

    void updateBuffers() {

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        if (isSmoothShading) {
            glBufferSubData(GL_ARRAY_BUFFER, vSize + cSize + tSize, nSize, normalVertex);
        }
        else {
            glBufferSubData(GL_ARRAY_BUFFER, vSize + cSize + tSize, nSize, normalFace);
        }
    }

    void draw(Shader* shader) {
        shader->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };

};


#endif
