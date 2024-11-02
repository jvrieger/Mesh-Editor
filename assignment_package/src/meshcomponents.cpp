#include "meshcomponents.h"

// init static member variables
int Vertex::nextID = 0;
int Face::nextID = 0;
int HalfEdge::nextID = 0;

Vertex::Vertex(const glm::vec3& pos)
    : QListWidgetItem(), position(pos), edge(nullptr), id(nextID++), isOriginal(true) {}

int Vertex::getID() const {
    return id;
}

void Vertex::setHE(HalfEdge* e) {
    this->edge = e;
    e->vert = this;
}

Face::Face(const glm::vec3& col)
    : QListWidgetItem(), edge(nullptr), color(col), id(nextID++) {}

int Face::getID() const {
    return id;
}

void Face::setEdge(HalfEdge* edge) {
    this->edge = edge;
    edge->face = this;
}

HalfEdge::HalfEdge()
    : QListWidgetItem(), next(nullptr), sym(nullptr), face(nullptr), vert(nullptr), id(nextID++), isOriginal(true) {}

int HalfEdge::getID() const {
    return id;
}

void HalfEdge::setNext(HalfEdge* nextEdge) {
    this->next = nextEdge;
}

void HalfEdge::setSym(HalfEdge* symEdge) {
    this->sym = symEdge;
    symEdge->sym = this;
}

void HalfEdge::setFace(Face* f) {
    this->face = f;
    f->edge = this;
}

void HalfEdge::setVertex(Vertex* vertex) {
    this->vert = vertex;
    vertex->edge = this;
}

VertexDisplay::VertexDisplay(OpenGLContext* context)
    : Drawable(context), representedVertex(nullptr) {}


void VertexDisplay::updateVertex(Vertex* v) {
    representedVertex = v;
    initializeAndBufferGeometryData(); //rebuffer data when new v is selected
}

void VertexDisplay::initializeAndBufferGeometryData() {
    if (representedVertex == nullptr) {
        return;
    }

    std::vector<glm::vec3> pos = { representedVertex->position }; //hold vertex pos to render as a point
    std::vector<unsigned int> indices = { 0 };
    std::vector<float> col = { 1, 1, 1 };

    indexBufferLength = 1; // 1 vertex to draw

    generateBuffer(POSITION);
    bindBuffer(POSITION);
    bufferData(POSITION, pos);

    generateBuffer(COLOR);
    bindBuffer(COLOR);
    bufferData(COLOR, col);

    generateBuffer(INDEX);
    bindBuffer(INDEX);
    bufferData(INDEX, indices);
}

GLenum VertexDisplay::drawMode() {
    // return GL_POINTS since were drawing a single point
    return GL_POINTS;
}

FaceDisplay::FaceDisplay(OpenGLContext* context)
    : Drawable(context), representedFace(nullptr) {}

void FaceDisplay::updateFace(Face* f) {
    representedFace = f;
    initializeAndBufferGeometryData(); // rebuffer geometry data when face is updated
}

void FaceDisplay::initializeAndBufferGeometryData() {
    if (representedFace == nullptr) {
        return;
    }

    std::vector<glm::vec3> pos; // To store n vertex positions for n-gon
    HalfEdge* start = representedFace->edge;
    HalfEdge* current = start;

    // traverse all half-edges of the face to collect vertex positions
    do {
        pos.push_back(current->vert->position);
        current = current->next;
    } while (current != start);

    std::vector<unsigned int> indices(pos.size());
    for (unsigned int i = 0; i < indices.size(); ++i) {
        indices[i] = i; // index setup for the n-gon
    }

    glm::vec3 faceColor = representedFace->color;
    glm::vec3 outlineColor = glm::vec3(1.0f) - faceColor;
    std::vector<glm::vec3> colors(pos.size(), outlineColor);

    indexBufferLength = pos.size(); // n vertices to draw

    // buffer vertex data
    generateBuffer(POSITION);
    bindBuffer(POSITION);
    bufferData(POSITION, pos);

    generateBuffer(COLOR);
    bindBuffer(COLOR);
    bufferData(COLOR, colors);

    generateBuffer(INDEX);
    bindBuffer(INDEX);
    bufferData(INDEX, indices);
}

GLenum FaceDisplay::drawMode() {
    return GL_LINE_LOOP; // draw face outline (edges)
}

HalfEdgeDisplay::HalfEdgeDisplay(OpenGLContext* context)
    : Drawable(context), representedHE(nullptr) {}

void HalfEdgeDisplay::updateHE(HalfEdge* he) {
    representedHE = he;
    initializeAndBufferGeometryData(); // rebuffer geometry data when half-edge is updated
}

void HalfEdgeDisplay::initializeAndBufferGeometryData() {
    if (representedHE == nullptr) {
        return;
    }

    std::vector<glm::vec3> pos(2); // two positions: start and end of the half-edge

    pos[0] = representedHE->sym->vert->position;      // Start vertex position
    pos[1] = representedHE->vert->position; // End vertex position (vertex HE is pointing to)
    //change next to sym

    std::vector<unsigned int> indices = { 0, 1 };

    // color vector for the gradient from red to yellow
    std::vector<glm::vec3> colors = {
        glm::vec3(1.0f, 0.0f, 0.0f), // red at the first vertex
        glm::vec3(1.0f, 1.0f, 0.0f)  // yellow at the second vertex
    };
    indexBufferLength = 2; // 2 vertices/points to draw

    // buffer vertex data
    generateBuffer(POSITION);
    bindBuffer(POSITION);
    bufferData(POSITION, pos); // Two points

    generateBuffer(COLOR);
    bindBuffer(COLOR);
    bufferData(COLOR, colors);

    generateBuffer(INDEX);
    bindBuffer(INDEX);
    bufferData(INDEX, indices);
}

GLenum HalfEdgeDisplay::drawMode() {
    return GL_LINES; // draw the HE as a line
}

