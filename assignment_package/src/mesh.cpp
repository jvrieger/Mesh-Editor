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

//load an obj file and make a mesh construct
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
    std::map<std::pair<int, int>, HalfEdge*> vertsToHEs; // map for setting up SYMs

    // read the file line by line
    while (std::getline(objFile, line)) {
        std::istringstream iss(line); //setup prefixxing to read lines more efficiently
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            // vertex position
            float x, y, z;
            iss >> x >> y >> z;
            createVertex(glm::vec3(x, y, z));

        } else if (prefix == "f") { //for every face
            // create new face with a random color
            glm::vec3 color = glm::vec3(
                static_cast<float>(arc4random()) / static_cast<float>(UINT32_MAX),
                static_cast<float>(arc4random()) / static_cast<float>(UINT32_MAX),
                static_cast<float>(arc4random()) / static_cast<float>(UINT32_MAX)
                );
            Face* facePtr = createFace(color);

            HalfEdge* firstHE = nullptr;
            HalfEdge* prevHE = nullptr;
            Vertex* behindVert = nullptr; //set this equal to the vert pos value of the last triplet in the f line
                                          //ie: last vert in the face

            // Store all triplets in a temporary container
            std::vector<std::string> triplets;
            std::string triplet;
            while (iss >> triplet) {
                triplets.push_back(triplet);
            }
            // Get the last triplet to set behindVert
            if (!triplets.empty()) {
                std::istringstream lastTripletStream(triplets.back());
                std::string posIndexStr, uvIndexStr, normIndexStr;
                std::getline(lastTripletStream, posIndexStr, '/');
                std::getline(lastTripletStream, uvIndexStr, '/');
                std::getline(lastTripletStream, normIndexStr, '/');

                int lastPosIndex = std::stoi(posIndexStr) - 1; // OBJ indices are 1-based
                behindVert = vertices[lastPosIndex].get(); // Set behindVert to the last vertex in the face
            }

            for (const std::string& triplet : triplets) { //for every triplet (vertex) in the face
                std::istringstream tripletStream(triplet);
                std::string posIndexStr, uvIndexStr, normIndexStr;
                int posIndex; //will be set to the pos of each vertex in this face as we loop

                // parse the triplet format: "position/uv/normal"
                std::getline(tripletStream, posIndexStr, '/');
                std::getline(tripletStream, uvIndexStr, '/');
                std::getline(tripletStream, normIndexStr, '/');

                posIndex = std::stoi(posIndexStr) - 1; // OBJ indices are 1-based
                Vertex* v = vertices[posIndex].get(); //retrieve this vertex

                // create a new half-edge
                HalfEdge* edgePtr = createHalfEdge();
                edgePtr->setVertex(v); //set v to be the vertex this edge is pointing to
                edgePtr->setFace(facePtr);

                // setup symmetry pointers
                if (firstHE == nullptr) { //this will run for only the first edge in the face
                    auto vertPair = std::make_pair(
                        std::min(v->id, behindVert->id),
                        std::max(v->id, behindVert->id)
                        );

                    if (vertsToHEs.find(vertPair) != vertsToHEs.end()) { //if there is a valid sym pair
                        HalfEdge* symEdge = vertsToHEs[vertPair];
                        edgePtr->setSym(symEdge); //sets both syms
                    } else { //the sym isn't here yet
                        vertsToHEs[vertPair] = edgePtr;
                    }
                } else { //run for all other edges because prevHE will be set
                    auto vertPair = std::make_pair(
                        std::min(v->id, prevHE->vert->id),
                        std::max(v->id, prevHE->vert->id)
                        );

                    if (vertsToHEs.find(vertPair) != vertsToHEs.end()) { //if there is a valid sym pair
                        HalfEdge* symEdge = vertsToHEs[vertPair];
                        edgePtr->setSym(symEdge); //sets both syms
                    } else { //the sym isn't here yet
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

//populate the QListWidgets with all the mesh components on loading an obj file
void Mesh::setListWidgets(QListWidget* vertsWidget, QListWidget* facesWidget, QListWidget* halfEdgesWidget) {
    vertsWidget->clear();
    facesWidget->clear();
    halfEdgesWidget->clear();

    // loop through vertices and assign a unique label
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vertex* vert = vertices[i].get();
        vert->setText(QString("Vertex %1").arg(vert->id));
        vertsWidget->addItem(vert);
    }

    //loop through faces and assign a unique label
    for (size_t i = 0; i < faces.size(); ++i) {
        Face* face = faces[i].get();
        face->setText(QString("Face %1").arg(face->id));
        facesWidget->addItem(face);
    }

    // loop through half-edges and assign a unique label
    for (size_t i = 0; i < halfEdges.size(); ++i) {
        HalfEdge* he = halfEdges[i].get();
        he->setText(QString("HalfEdge %1").arg(he->id));
        halfEdgesWidget->addItem(he);
    }
}

//split an edge by adding a vertex and 2 new halfedges
void Mesh::splitEdge(HalfEdge* selectedHE, QListWidget* vertsWidget, QListWidget* halfEdgesWidget) {
    if (!selectedHE) return; // do nothing if no HalfEdge is selected

    HalfEdge* HE1 = selectedHE->sym;
    HalfEdge* HE2 = selectedHE;
    Vertex* V1 = HE1->vert;
    Vertex* V2 = HE2->vert;
    HalfEdge* newHE1 = createHalfEdge();
    HalfEdge* newHE2 = createHalfEdge();

    // create a new vert at the midpoint bw two vertices
    glm::vec3 midPos = ((V1->position) + (V2->position)) / 2.0f; //average of start and end position matrices
    Vertex* midVertex = createVertex(midPos); //not yet attached to any part of the mesh

    // update references in the new HalfEdges
    newHE1->setVertex(V1);
    newHE2->setVertex(V2);
    newHE1->setNext(HE1->next);
    newHE2->setNext(HE2->next);
    newHE1->setSym(HE2);
    newHE2->setSym(HE1);
    newHE1->setFace(HE1->face);
    newHE2->setFace(HE2->face);

    //adjust next and vert pointers of original HEs
    HE1->setVertex(midVertex);
    HE2->setVertex(midVertex);
    HE1->setNext(newHE1);
    HE2->setNext(newHE2);

    //add the new mesh components to their respective list widgets
    midVertex->setText(QString("Vertex %1").arg(midVertex->getID()));
    vertsWidget->addItem(midVertex);

    newHE1->setText(QString("HalfEdge %1").arg(newHE1->getID()));
    halfEdgesWidget->addItem(newHE1);
    newHE2->setText(QString("HalfEdge %1").arg(newHE2->getID()));
    halfEdgesWidget->addItem(newHE2);
}

//helper function to count n edges in face
int Mesh::countEdgesInFace(Face* face) {
    if (!face) return 0;
    int count = 0;
    HalfEdge* start = face->edge;
    HalfEdge* current = start;
    do {
        count++;
        current = current->next;
    } while (current != start);
    return count;
}

//segment a face into 2+ faces where all faces are triangles using fan triangulation
void Mesh::triangulateFace(Face* face, QListWidget* facesWidget, QListWidget* halfEdgesWidget) {
    // validate the face, count edges/verts
    if (!face) return;
    int numEdges = countEdgesInFace(face);
    if (numEdges < 3) return;

    HalfEdge* startHE = face->edge;
    Vertex* v1 = startHE->vert; //start vertex to connect to all others

    HalfEdge* currentHE = startHE->next;
    for (int i = 0; i < numEdges - 3; ++i) {
        // Get the two vertices to form a triangle with the startVertex
        Vertex* v2 = currentHE->vert;
        Vertex* v3 = currentHE->next->vert;

        // Create new half-edges for the diagonal
        HalfEdge* diagonal1 = createHalfEdge();
        HalfEdge* diagonal2 = createHalfEdge();
        diagonal1->setVertex(v3); // Diagonal1 points from v1 to v3
        diagonal2->setVertex(v1); // Diagonal2 points from v3 to v1
        diagonal1->setSym(diagonal2); // Set them as symmetric pairs

        // Create one new triangular face (reuse the original face for the other triangle)
        glm::vec3 newFaceColor = glm::vec3(
            static_cast<float>(arc4random()) / static_cast<float>(UINT32_MAX),
            static_cast<float>(arc4random()) / static_cast<float>(UINT32_MAX),
            static_cast<float>(arc4random()) / static_cast<float>(UINT32_MAX)
            );
        Face* newFace = createFace(newFaceColor); // Use the same color as the original face

        // Set up the original face's remaining half-edges to be part of the next iteration
        startHE->setNext(diagonal1);
        diagonal1->setNext(currentHE->next->next);
        diagonal1->setFace(face);

        // Update half-edges to create the next triangle (v1, v2, v3) using the new face
        currentHE->setNext(currentHE->next);
        currentHE->next->setNext(diagonal2);
        diagonal2->setNext(currentHE);
        diagonal2->setFace(newFace);
        currentHE->setFace(newFace);
        currentHE->next->setFace(newFace);
        newFace->setEdge(currentHE);

        // Advance to the next half-edge for the next iteration
        currentHE = diagonal1;

        //add the new mesh components to their respective list widgets
        diagonal1->setText(QString("HalfEdge %1").arg(diagonal1->getID()));
        halfEdgesWidget->addItem(diagonal1);
        diagonal2->setText(QString("HalfEdge %1").arg(diagonal2->getID()));
        halfEdgesWidget->addItem(diagonal2);

        newFace->setText(QString("Face %1").arg(newFace->getID()));
        facesWidget->addItem(newFace);
    }

    // Final triangle between startVertex, the last two remaining vertices of the face
    // Update the original face's half-edges to form the last triangle
    currentHE->setNext(currentHE->next);
    currentHE->next->setNext(startHE);
    startHE->setNext(currentHE);

    // Update the face of the original edges to the original face
    currentHE->setFace(face);
    currentHE->next->setFace(face);
    startHE->setFace(face);

    // Set the edge of the original face to startHE
    face->setEdge(startHE);

}

//helper function to create centroid position matrix for every face (center of each face), add to map
Vertex* Mesh::createCentroid(Face *face) {
    glm::vec3 centroidPos(0.0f);
    int numVertices = 0;
    HalfEdge* start = face->edge;
    HalfEdge* current = start;

    // Calculate centroid by averaging the positions of all vertices of the face
    do {
        centroidPos += current->vert->position;
        current = current->next;
        ++numVertices;
    } while (current != start);

    centroidPos /= static_cast<float>(numVertices);

    return createVertex(centroidPos);
}

std::pair<HalfEdge*, HalfEdge*> Mesh::makeEdgePair(HalfEdge* he1, HalfEdge* he2) {
    return (he1 < he2) ? std::make_pair(he1, he2) : std::make_pair(he2, he1);
}

//helper function to calculate the midpoint between two vertices on an edge and two centroids on a face
Vertex* Mesh::createEdgePoint(HalfEdge* he, Vertex* centroid1, Vertex* centroid2, QListWidget* vertsWidget, QListWidget* halfEdgesWidget) {
    // Calculate edge midpoint by averaging two vertices and adjacent face centroids
    glm::vec3 vertPos1 = he->vert->position;
    glm::vec3 vertPos2 = he->sym->vert->position;
    glm::vec3 centPos1 = centroid1->position;
    glm::vec3 centPos2 = centroid2->position;
    glm::vec3 edgeMidpointPos = (vertPos1 + vertPos2 + centPos1 + centPos2) / 4.0f;
    Vertex* edgeMidpoint = createVertex(edgeMidpointPos);

    //setup new halfedges and reroute pointers/paths
    HalfEdge* HE1 = he->sym;
    HalfEdge* HE2 = he;
    Vertex* V1 = HE1->vert;
    Vertex* V2 = HE2->vert;
    HalfEdge* newHE1 = createHalfEdge();
    HalfEdge* newHE2 = createHalfEdge();
    newHE1->isOriginal = false; //mark new HEs as new so we don't keep splitting them
    newHE2->isOriginal = false;
    HE1->isOriginal = false;
    HE2->isOriginal = false;

    // update references in the new HalfEdges
    newHE1->setVertex(V1);
    newHE2->setVertex(V2);
    newHE1->setNext(HE1->next);
    newHE2->setNext(HE2->next);
    newHE1->setSym(HE2);
    newHE2->setSym(HE1);
    newHE1->setFace(HE1->face);
    newHE2->setFace(HE2->face);

    //adjust next and vert pointers of original HEs
    HE1->setVertex(edgeMidpoint);
    HE2->setVertex(edgeMidpoint);
    HE1->setNext(newHE1);
    HE2->setNext(newHE2);

    //add the new mesh components to their respective list widgets
    edgeMidpoint->setText(QString("VertexMidPoint %1").arg(edgeMidpoint->getID()));
    vertsWidget->addItem(edgeMidpoint);

    newHE1->setText(QString("HalfEdge %1").arg(newHE1->getID()));
    halfEdgesWidget->addItem(newHE1);
    newHE2->setText(QString("HalfEdge %1").arg(newHE2->getID()));
    halfEdgesWidget->addItem(newHE2);

    return edgeMidpoint;
}

//helper function populates all Halfedges that point to this vertex into connectedEdges
void Mesh::getConnectedEdges(Vertex* v, std::vector<HalfEdge*> *connectedEdges) {
    // Start with one of the half-edges pointing to this vertex
    HalfEdge* startEdge = v->edge;
    HalfEdge* currentEdge = startEdge;

    do {
        // Add the current half-edge to the list
        connectedEdges->push_back(currentEdge);

        // Move to the next half-edge around this vertex
        currentEdge = currentEdge->next->sym; // Traverse to the next half-edge with the same end vertex
    } while (currentEdge != startEdge);

}

//helper function populates all faces this vertex is a part of into connectedFaces
void Mesh::getConnectedFaces(Vertex* v, std::vector<Face*> *connectedFaces) {
    // Start with one of the half-edges pointing to this vertex
    HalfEdge* startEdge = v->edge;
    HalfEdge* currentEdge = startEdge;

    do {
        // Add the face of the current half-edge to the list
        connectedFaces->push_back(currentEdge->face);

        // Move to the next half-edge around this vertex
        currentEdge = currentEdge->next->sym; // Traverse to the next half-edge that points to this vert
    } while (currentEdge != startEdge);

}

//helper function easily get all original half edges around face
void Mesh::getOriginalHalfEdges(Face* face, std::vector<HalfEdge*> *originalHalfEdges) {
    // Start from the starting edge of the face
    HalfEdge* startEdge = face->edge;
    HalfEdge* currentEdge = startEdge;

    do {
        originalHalfEdges->push_back(currentEdge);
        currentEdge = currentEdge->next;  // Move to the next half-edge in the loop
    } while (currentEdge != startEdge);  // Stop when we complete the loop
}

void Mesh::catmullClarkSubdivide(QListWidget* vertsWidget, QListWidget* facesWidget, QListWidget* halfEdgesWidget) {

    std::unordered_map<Face*, Vertex*> faceToCentroid; //holds face/centroid pairs
    std::unordered_map<std::pair<HalfEdge*, HalfEdge*>, Vertex*, EdgePairHash> edgeMidpoints; //holds edge/midpoint pairs

    //CALC CENTROIDS
    for (const auto& facePtr : faces) {
        Face* face = facePtr.get();
        Vertex* newCentVert = createCentroid(face);
        newCentVert->isOriginal = false; //this is not one of the mesh's original verts
        //compute and store centroid in map for all faces
        faceToCentroid[face] = newCentVert;

        std::cout << "CENTROID: " << newCentVert->position.x << ' ' << newCentVert->position.y << ' ' << newCentVert->position.z << std::endl;
        newCentVert->setText(QString("VertexCentroid %1").arg(newCentVert->getID()));
        vertsWidget->addItem(newCentVert);
    }
    //CALC MIDPOINTS
    std::vector<HalfEdge*> originalHalfEdges;
    for (const auto& hePtr : halfEdges) {
        if (hePtr && hePtr->isOriginal) {
            originalHalfEdges.push_back(hePtr.get());
        }
    }
    for (HalfEdge* halfEdge : originalHalfEdges) {
        if (!halfEdge->isOriginal) {
            //do nothing if this is a new HE, skip splitting
        } else {
            HalfEdge* symEdge = halfEdge->sym;
            auto edgePair = makeEdgePair(halfEdge, symEdge); //make edgepair key for map

            // Check if midpoint already computed, if not, compute and store in map
            if (edgeMidpoints.find(edgePair) != edgeMidpoints.end()) continue; //if already in map skip

            Vertex* newEdgeMidpoint = createEdgePoint(halfEdge, faceToCentroid[halfEdge->face], faceToCentroid[symEdge->face], vertsWidget, halfEdgesWidget);
            newEdgeMidpoint->isOriginal = false; //this is not one of the mesh's original verts
            edgeMidpoints[edgePair] = newEdgeMidpoint;

            std::cout << "MIDPOINT: " << newEdgeMidpoint->position.x  << ' ' << newEdgeMidpoint->position.y << ' ' << newEdgeMidpoint->position.z << std::endl;
        }
    }
    //UPDATE OG VERT POS to smoothed pos based on centroids and midpoints
    for (const auto& vertPtr : vertices) {
        Vertex* v = vertPtr.get();
        if (!v->isOriginal) continue; // skip non-original vertices like centroids and edge midpoints

        std::vector<HalfEdge*> connectedEdges; //holds all HEs that point to this v
        std::vector<Face*> connectedFaces; //holds all faces that this v is a part of
        getConnectedEdges(v, &connectedEdges);
        getConnectedFaces(v, &connectedFaces);

        //variables in v' calculation formula
        float n = connectedEdges.size(); // number of adjacent midpoints (= to num of adjacent edges)
        glm::vec3 sumE(0.0f); // Sum of edge midpoints
        glm::vec3 sumF(0.0f); // Sum of face centroids
        glm::vec3 vPos = v->position; //v's original pos

        //traverse all HEs connected to this vertex
        for (HalfEdge* he : connectedEdges) {
            sumF += faceToCentroid[he->face]->position; //sum up connected face centroid points

            auto edgePair = makeEdgePair(he->sym, he->sym->next->sym); //make key into map
            sumE += edgeMidpoints[edgePair]->position; //sum up connected midpoints
        }

        // compute the new position for the vertex
        glm::vec3 newPosition = ((n - 2.0f) * vPos / n) + (sumE / (n * n)) + (sumF / (n * n));
        v->position = newPosition;
        std::cout << "NEWVERTPOS: " << v->position.x  << ' ' << v->position.y << ' ' << v->position.z << std::endl;

    }

    //QUADRANGULATE
    std::vector<Face*> originalFaces;
    for (const auto& facePtr : faces) {
        if (facePtr) {
            originalFaces.push_back(facePtr.get());
        }
    }
    for (Face* ogFace : originalFaces) {
        std::vector<HalfEdge*> originalHalfEdges;
        getOriginalHalfEdges(ogFace, &originalHalfEdges);
        int n = originalHalfEdges.size()/2.0;  // Number of vertices/HEs in the original face (and thus quadrangles to create)
        HalfEdge* firstHE4 = nullptr; //the first he4 (points to vPrev) set, to be sym with last faces he3 (he3 and he4 of adjacest faces are syms)
        HalfEdge* prevHE3 = nullptr; //the previous face's he3 to be sym with next he4
        HalfEdge* prevHE2 = nullptr; //HE pointing to vPrev
        HalfEdge* nextHE1 = ogFace->edge;

        for (int i = 0; i < n-1; ++i) { //for every quadrangle to create except the last

            std::cout << "CREATING QUAD: " << i  << ' ' << ogFace->id << ' ' << nextHE1->id << std::endl;

            // gather elements for the i-th quadrangle and create new half-edges for this quadrangle
            HalfEdge* he1 = nextHE1;  // Between vPrev and v
            Vertex* v = he1->vert;  // ogFace will point to an edge that points to an Original vertex
            he1->setVertex(v);

            HalfEdge* he2 = he1->next;  // Between v and vNext
            Vertex* vNext = edgeMidpoints[makeEdgePair(he2, he2->next->sym)]; //midpoint on edge pointing from v
            he2->setVertex(vNext);

            HalfEdge* he3 = createHalfEdge();  // Between vNext and vCent
            Vertex* vCent = faceToCentroid[ogFace];  // This face's centroid
            he3->setVertex(vCent);

            HalfEdge* he4 = createHalfEdge();  // Between vCent and vPrev
            HalfEdge* tempHe = he1;
            if (prevHE2 == nullptr) {
                //first quadrangle, traverse boundary until we reach the edge right before he1 (ie last quad's he2, pointing to this vPrev)
                do {
                    prevHE2 = tempHe;
                    tempHe = tempHe->next;
                } while (tempHe != he1);
            }
            Vertex* vPrev = edgeMidpoints[makeEdgePair(he1->sym, prevHE2)]; //midpoint on edge pointing to v
            he4->setVertex(vPrev);

            nextHE1 = he2->next; //set the first half edge for the next quadrangle -- an edge that points to an og vert

            // Link half-edges to form a cycle
            he1->setNext(he2);
            he2->setNext(he3);
            he3->setNext(he4);
            he4->setNext(he1);

            // Create new quad face (reuse the original face for the last quad)
            glm::vec3 newFaceColor = glm::vec3(
                static_cast<float>(arc4random()) / static_cast<float>(UINT32_MAX),
                static_cast<float>(arc4random()) / static_cast<float>(UINT32_MAX),
                static_cast<float>(arc4random()) / static_cast<float>(UINT32_MAX)
                );
            Face* quadFace = createFace(newFaceColor);
            he2->setFace(quadFace);
            he3->setFace(quadFace);
            he4->setFace(quadFace);
            quadFace->setEdge(he1); //new face's start edge is he1, which points to an original vertex

            //set internal syms (he3 and 4)
            prevHE3 = he3; //set prev he3 of for the next face's he4 sym
            prevHE2 = he2; //set prev he2 for the next face's vPrev
            if (firstHE4 == nullptr) { //if this is the first quadrangle
                firstHE4 = he4; //set the first he4 for this ogFace (in last new face, set this as sym w he3)
            } else {
                he4->setSym(prevHE3); //for all but first face, sym curr he4 and prev he3
            }

            //add to widgets: new new half edges, new face
            he3->setText(QString("HalfEdge %1").arg(he3->getID()));
            halfEdgesWidget->addItem(he3);
            he4->setText(QString("HalfEdge %1").arg(he4->getID()));
            halfEdgesWidget->addItem(he4);

            quadFace->setText(QString("Face %1").arg(quadFace->getID()));
            facesWidget->addItem(quadFace);
        }
        std::cout << "CREATING QUAD: 4"  << ' ' << ogFace->id << ' ' << nextHE1->id << std::endl;

        //Deal with the last quadrangle using the original face
        // gather elements for the i-th quadrangle and create new half-edges for this quadrangle
        HalfEdge* he1 = nextHE1;  // Between vPrev and v
        Vertex* v = he1->vert;  // ogFace will point to an edge that points to an Original vertex
        he1->setVertex(v);

        HalfEdge* he2 = he1->next;  // Between v and vNext
        Vertex* vNext = edgeMidpoints[makeEdgePair(he2, he2->next->sym)]; //midpoint on edge pointing from v
        he2->setVertex(vNext);

        HalfEdge* he3 = createHalfEdge();  // Between vNext and vCent
        Vertex* vCent = faceToCentroid[ogFace];  // This face's centroid
        he3->setVertex(vCent);

        HalfEdge* he4 = createHalfEdge();  // Between vCent and vPrev
        Vertex* vPrev = edgeMidpoints[makeEdgePair(he1->sym, prevHE2)]; //midpoint on edge pointing to v
        he4->setVertex(vPrev);

        // Link half-edges to form a cycle
        he1->setNext(he2);
        he2->setNext(he3);
        he3->setNext(he4);
        he4->setNext(he1);

        he2->setFace(ogFace);
        he3->setFace(ogFace);
        he4->setFace(ogFace);
        ogFace->setEdge(he1); //og face's start edge is he1, which points to an original vertex

        he3->setSym(firstHE4);
        he4->setSym(prevHE3);

        //he1 and he2 are boundary edges, he3 and he4 are internal
    }

    //once all vertices are part of the mesh (end of subdivision), set all verts->isOriginal(true)
    for (const auto& vertPtr : vertices) {
        Vertex* vert = vertPtr.get();
        vert->isOriginal = true;
    }
    for (const auto& hePtr : halfEdges) {
        HalfEdge* he = hePtr.get();
        he->isOriginal = true;
    }
}
