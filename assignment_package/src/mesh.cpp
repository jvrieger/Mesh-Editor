#include "mesh.h"

// constructor with Drawable initialization
Mesh::Mesh(OpenGLContext* context)
    : Drawable(context) {}

GLenum Mesh::drawMode() {
    return GL_TRIANGLES;
}

//create new vert and store in vector
Vertex* Mesh::createVertex(const glm::vec3& position) {
    vertices.push_back(mkU<Vertex>(position));
    return vertices.back().get();
}

//create new face and store in vec
Face* Mesh::createFace(const glm::vec3& color) {
    faces.push_back(mkU<Face>(color));
    return faces.back().get();
}

//crete new HE, store in vec
HalfEdge* Mesh::createHalfEdge() {
    halfEdges.push_back(mkU<HalfEdge>());
    return halfEdges.back().get();
}

//implement drawable's initAndBufferGeomData
void Mesh::initializeAndBufferGeometryData() {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;
    std::vector<unsigned int> indices;

    for (const auto& face : faces) {
        HalfEdge* start = face->edge;
        HalfEdge* edge = start;
        int startIndex = static_cast<int>(positions.size());

        do {
            // store positions/normals/colors for each vertex of the face
            positions.push_back(edge->vert->position);
            normals.push_back(glm::normalize(glm::cross(
                edge->next->vert->position - edge->vert->position,
                edge->next->next->vert->position - edge->next->vert->position
                )));
            //edge case of the normal is 0 0 0 ?
            colors.push_back(face->color);
            edge = edge->next;

        } while (edge != start);

        // store indices for the triangles forming this face
        int numEdges = static_cast<int>(positions.size()) - startIndex;
        for (int i = 1; i < numEdges - 1; ++i) {
            indices.push_back(startIndex);
            indices.push_back(startIndex + i);
            indices.push_back(startIndex + i + 1);
        }
    }

    indexBufferLength = indices.size();

    // setup the VBOs with the data
    setupVBOs();

    bindBuffer(POSITION);
    bufferData(POSITION, positions);

    bindBuffer(NORMAL);
    bufferData(NORMAL, normals);

    bindBuffer(COLOR);
    bufferData(COLOR, colors);

    bindBuffer(INDEX);
    bufferData(INDEX, indices);
}

//setup VBOs
void Mesh::setupVBOs() {
    generateBuffer(POSITION);
    generateBuffer(NORMAL);
    generateBuffer(COLOR);
    generateBuffer(INDEX);
}

void Mesh::loadOBJ(const QString &filename) {
    // clear existing mesh data
    vertices.clear();
    faces.clear();
    halfEdges.clear();

    std::string filePath = filename.toStdString();
    std::ifstream objFile(filePath); //check for file opening errors
    if (!objFile.is_open()) {
        std::cerr << "Error: Could not open OBJ file." << std::endl;
        return;
    }

    std::string line;
    std::vector<glm::vec3> tempPositions; // temp storage for vertex positions
    std::vector<glm::vec2> tempUVs;       // temp storage for UVs
    std::vector<glm::vec3> tempNormals;   // temp storage for normals
    std::map<std::pair<int, int>, HalfEdge*> vertsToHEs; // map for setting up SYMs
    std::map<int, Vertex*> uniqueVertices; //map to track unique vertices

    // read the file line by line
    while (std::getline(objFile, line)) {
        std::istringstream iss(line); //setup prefixxing to read lines more efficiently
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            // vertex position
            float x, y, z;
            iss >> x >> y >> z;
            tempPositions.push_back(glm::vec3(x, y, z));
        } else if (prefix == "vt") {
            // texture coord
            float u, v;
            iss >> u >> v;
            tempUVs.push_back(glm::vec2(u, v));
        } else if (prefix == "vn") {
            // normal vector
            float nx, ny, nz;
            iss >> nx >> ny >> nz;
            tempNormals.push_back(glm::vec3(nx, ny, nz));
        } else if (prefix == "f") {
            // create new face with a random color
            glm::vec3 color = glm::vec3(
                static_cast<float>(arc4random()) / static_cast<float>(UINT32_MAX),
                static_cast<float>(arc4random()) / static_cast<float>(UINT32_MAX),
                static_cast<float>(arc4random()) / static_cast<float>(UINT32_MAX)
                );
            Face* facePtr = createFace(color);

            HalfEdge* firstHE = nullptr;
            HalfEdge* prevHE = nullptr;
            Vertex* behindVert = nullptr;

            std::string triplet;
            while (iss >> triplet) {
                std::istringstream tripletStream(triplet);
                std::string posIndexStr, uvIndexStr, normIndexStr;
                int posIndex, uvIndex, normIndex;

                // parse the triplet format: "position/uv/normal"
                std::getline(tripletStream, posIndexStr, '/');
                std::getline(tripletStream, uvIndexStr, '/');
                std::getline(tripletStream, normIndexStr, '/');

                posIndex = std::stoi(posIndexStr) - 1; // OBJ indices are 1-based
                uvIndex = std::stoi(uvIndexStr) - 1;
                normIndex = std::stoi(normIndexStr) - 1;

                // create/retrieve vertex, checking to ensure its unique
                Vertex* v = nullptr;
                if (uniqueVertices.find(posIndex) != uniqueVertices.end()) {
                    // Use existing vertex
                    v = uniqueVertices[posIndex];
                } else {
                    // Create a new vertex and store it in the map
                    v = createVertex(tempPositions[posIndex]);
                    uniqueVertices[posIndex] = v;
                }

                // create a new half-edge
                HalfEdge* edgePtr = createHalfEdge();
                edgePtr->setVertex(v);
                edgePtr->setFace(facePtr);

                // setup symmetry pointers
                if (behindVert != nullptr) {
                    auto vertPair = std::make_pair(
                        std::min(v->id, behindVert->id),
                        std::max(v->id, behindVert->id)
                        );

                    if (vertsToHEs.find(vertPair) != vertsToHEs.end()) {
                        HalfEdge* symEdge = vertsToHEs[vertPair];
                        edgePtr->setSym(symEdge);
                        symEdge->setSym(edgePtr);
                    } else {
                        vertsToHEs[vertPair] = edgePtr;
                    }
                }

                // set next and prev pointers
                if (firstHE == nullptr) {
                    firstHE = edgePtr;
                } else {
                    prevHE->setNext(edgePtr);
                }

                prevHE = edgePtr;
                behindVert = v;
            }

            // close the loop for the face
            if (firstHE && prevHE) {
                prevHE->setNext(firstHE);
            }

            // set one of the face's half-edges
            facePtr->setEdge(firstHE);
        }
    }

    objFile.close();

    // set up VBOs based on the loaded mesh data
    initializeAndBufferGeometryData();
}

void Mesh::setListWidgets(QListWidget* vertsWidget, QListWidget* facesWidget, QListWidget* halfEdgesWidget) {
    vertsWidget->clear();
    facesWidget->clear();
    halfEdgesWidget->clear();

    // loop through vertices and assign a unique label
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vertex* vert = vertices[i].get();
        vert->setText(QString("Vertex %1").arg(i));
        vertsWidget->addItem(vert);
    }

    //loop through faces and assign a unique label
    for (size_t i = 0; i < faces.size(); ++i) {
        Face* face = faces[i].get();
        face->setText(QString("Face %1").arg(i));
        facesWidget->addItem(face);
    }

    // loop through half-edges and assign a unique label
    for (size_t i = 0; i < halfEdges.size(); ++i) {
        HalfEdge* he = halfEdges[i].get();
        he->setText(QString("HalfEdge %1").arg(i));
        halfEdgesWidget->addItem(he);
    }
}
