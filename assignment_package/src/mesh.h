#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <map>
#include <QListWidget>
#include <QListWidgetItem>
#include "meshcomponents.h"
#include "drawable.h"
#include "utils.h"
#include "mainwindow.h"

struct EdgePairHash {
    std::size_t operator()(const std::pair<HalfEdge*, HalfEdge*>& edgePair) const {
        return std::hash<HalfEdge*>()(edgePair.first) ^ std::hash<HalfEdge*>()(edgePair.second);
    }
};

class Mesh : public Drawable {
public:
    Mesh(OpenGLContext* context);

    //override drawable's funcs
    void initializeAndBufferGeometryData() override;
    GLenum drawMode() override;

    //load mesh
    void loadOBJ(const QString &filename);
    void setListWidgets(QListWidget* vertsWidget, QListWidget* facesWidget, QListWidget* halfEdgesWidget); //populate QListWidgetItems

    //catmullclark/subdivision operations
    void splitEdge(HalfEdge* selectedHE, QListWidget* vertsWidget, QListWidget* halfEdgesWidget);
    void triangulateFace(Face* face, QListWidget* facesWidget, QListWidget* halfEdgesWidget);
    void catmullClarkSubdivide(QListWidget* vertsWidget, QListWidget* facesWidget, QListWidget* halfEdgesWidget);

private:
    //vectors which hold all mesh's components
    std::vector<uPtr<Vertex>> vertices;
    std::vector<uPtr<Face>> faces;
    std::vector<uPtr<HalfEdge>> halfEdges;

    //member funcs for creating meshcomponenets
    Vertex* createVertex(const glm::vec3& position);
    Face* createFace(const glm::vec3& color);
    HalfEdge* createHalfEdge();

    void setupVBOs(); //helper funcs
    int countEdgesInFace(Face* face);
    Vertex* createCentroid(Face* face);
    std::pair<HalfEdge*, HalfEdge*> makeEdgePair(HalfEdge* he1, HalfEdge* he2);
    Vertex* createEdgePoint(HalfEdge* he, Vertex* centroid1, Vertex* centroid2, QListWidget* vertsWidget, QListWidget* halfEdgesWidget);
    void getConnectedEdges(Vertex* v, std::vector<HalfEdge*> *connectedEdges);
    void getConnectedFaces(Vertex* v, std::vector<Face*> *connectedFaces);
    void getOriginalHalfEdges(Face* face, std::vector<HalfEdge*> *originalHalfEdges);
};

#endif // MESH_H
