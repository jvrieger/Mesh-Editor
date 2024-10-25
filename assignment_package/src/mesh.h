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


class Mesh : public Drawable {
public:
    Mesh(OpenGLContext* context);

    //override drawable's funcs
    void initializeAndBufferGeometryData() override;
    GLenum drawMode() override;

    //member funcs for creating meshcomponenets
    Vertex* createVertex(const glm::vec3& position);
    Face* createFace(const glm::vec3& color);
    HalfEdge* createHalfEdge();

    void loadOBJ(const QString &filename);
    void setListWidgets(QListWidget* vertsWidget, QListWidget* facesWidget, QListWidget* halfEdgesWidget); //populate QListWidgetItems

private:
    std::vector<uPtr<Vertex>> vertices;
    std::vector<uPtr<Face>> faces;
    std::vector<uPtr<HalfEdge>> halfEdges;

    void setupVBOs(); //helper func
};

#endif // MESH_H
