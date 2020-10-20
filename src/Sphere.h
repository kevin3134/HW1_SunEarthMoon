/**
 * Sphere class
 * 
 * 1, create a sphere using radium
 * 2, generate vertices and indices and bind to VAO, VBO
 * 3, draw sphere depend on mode (triangle or points)
**/


#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>

#include <vector>
#include <iostream>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


class Sphere
{
public:
    Sphere();
    ~Sphere();
    void init(GLuint vertexPositionID, GLfloat r_in);
    void cleanup();
    void draw(GLenum mode);

private:
    GLfloat r;
    int lats, longs;
    bool isInited;
    GLuint m_vao, m_vboVertex, m_vboIndex;
    int numsToDraw;
};


Sphere::Sphere()
{
    isInited = false;
    m_vao = 0;
    m_vboVertex = 0;
    m_vboIndex = 0;
    r = 1.0;
    lats = 40;
    longs = 40;
}


Sphere::~Sphere()
{}


void Sphere::init(GLuint vertexPositionID, GLfloat r_in)
{
    int i, j;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    int indicator = 0;
    r = r_in;
    for (i = 0; i <= lats; i++) {
        double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = glm::pi<double>() * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        for (j = 0; j <= longs; j++) {
            double lng = 2 * glm::pi<double>() * (double)(j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            vertices.push_back(x * zr0 * r);
            vertices.push_back(y * zr0 * r);
            vertices.push_back(z0 * r);
            indices.push_back(indicator);
            indicator++;

            vertices.push_back(x * zr1 * r);
            vertices.push_back(y * zr1 * r);
            vertices.push_back(z1 * r);
            indices.push_back(indicator);
            indicator++;
        }
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vboVertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(vertexPositionID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(vertexPositionID);

    glGenBuffers(1, &m_vboIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    numsToDraw = indices.size();

    isInited = true;
}


void Sphere::cleanup()
{
    if (!isInited) {
        return;
    }
    if (m_vboVertex) {
        glDeleteBuffers(1, &m_vboVertex);
    }
    if (m_vboIndex) {
        glDeleteBuffers(1, &m_vboIndex);
    }
    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
    }

    isInited = false;
    m_vao = 0;
    m_vboVertex = 0;
    m_vboIndex = 0;
    r = 0;
}


void Sphere::draw(GLenum mode)
{
    if (!isInited) {
        std::cout << "please call init() before draw()" << std::endl;
    }

    // draw sphere
    glBindVertexArray(m_vao);
    glEnable(GL_PRIMITIVE_RESTART);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
    glDrawArrays(mode, 0, 6 * lats * longs);
}

#endif // SPHERE_H