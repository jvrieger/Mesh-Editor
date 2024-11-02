#ifndef MESHCOMPONENTS_H
#define MESHCOMPONENTS_H

#include <glm/glm.hpp>
#include "drawable.h"
#include <QListWidgetItem>
#include <iostream>

class HalfEdge;
class Face;
class Vertex;

class Vertex : public QListWidgetItem {
public:
    glm::vec3 position;
    HalfEdge* edge; //pointer to one of the HEs pointing to this Vert
    int id; //unique int id for this Vert
    bool isOriginal; // true for original vertices, false for newly created ones

    Vertex(const glm::vec3& pos);

    int getID() const; //getter for id
    void setHE(HalfEdge* e);

private:
    static int nextID; //keep track of next avail id

};

class Face : public QListWidgetItem {
public:
    HalfEdge* edge; //one of the HEs that lie on this face
    glm::vec3 color; //this face's color, rgb
    int id; //unique int id for this face

    Face(const glm::vec3& col);

    int getID() const; //getter for id
    void setEdge(HalfEdge* edge);

private:
    static int nextID; //keep track of next avail id

};

class HalfEdge : public QListWidgetItem {
public:
    HalfEdge* next; //pointer to next HE in loop
    HalfEdge* sym; //pointer to symmetric HE
    Face* face; //pointer to face this HE lies in
    Vertex* vert; //pointer to vert at end of this HE
    int id; //unique int id for this HE
    bool isOriginal; // true for original HEs, false for newly created ones

    HalfEdge();

    int getID() const; //getter for id
    void setNext(HalfEdge* nextEdge);
    void setSym(HalfEdge* symEdge);
    void setFace(Face* face);
    void setVertex(Vertex* vert);

private:
    static int nextID; //keep track of next avail id

};

class VertexDisplay : public Drawable {
public:
    Vertex* representedVertex; //the vertex to be highlighted
    VertexDisplay(OpenGLContext* context); //inits opengl ontext from drawable
    void updateVertex(Vertex* v); //update vertex currently being displayed
    void initializeAndBufferGeometryData() override; //send vertex pos to GPU
    GLenum drawMode() override;

};

class FaceDisplay : public Drawable {
public:
    Face* representedFace; //the face to be highlighted
    FaceDisplay(OpenGLContext* context); //inits opengl ontext from drawable
    void updateFace(Face* f); //update face currently being displayed
    void initializeAndBufferGeometryData() override; //send face pos to GPU
    GLenum drawMode() override;

};

class HalfEdgeDisplay : public Drawable {
public:
    HalfEdge* representedHE; //the he to be highlighted
    HalfEdgeDisplay(OpenGLContext* context); //inits opengl ontext from drawable
    void updateHE(HalfEdge* he); //update vertex currently being displayed
    void initializeAndBufferGeometryData() override; //send vertex pos to GPU
    GLenum drawMode() override;

};

#endif // MESHCOMPONENTS_H
