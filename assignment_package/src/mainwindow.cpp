#include "mainwindow.h"
#include <ui_mainwindow.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    connect(ui->vertPosXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onVertexPositionChanged()));
    connect(ui->vertPosYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onVertexPositionChanged()));
    connect(ui->vertPosZSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onVertexPositionChanged()));
    connect(ui->faceRedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onFaceColorChanged()));
    connect(ui->faceGreenSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onFaceColorChanged()));
    connect(ui->faceBlueSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onFaceColorChanged()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionQuit_triggered() {
    QApplication::exit();
}

void MainWindow::on_openOBJ_clicked() {
    // open file dialog for selecting an OBJ file
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open OBJ File"), "",
                                                    tr("OBJ Files (*.obj);;All Files (*)"));

    // check if a file was selected
    if (!fileName.isEmpty()) {

        // load OBJ file
        ui->mygl->my_mesh.loadOBJ(fileName);

        ui->mygl->meshLoaded = true; //set meshLoaded as true so PaintGL() will paint our mesh
        ui->mygl->m_vertDisplay.representedVertex = nullptr; //reset selected mesh components
        ui->mygl->m_faceDisplay.representedFace = nullptr; //reset selected mesh components
        ui->mygl->m_HEDisplay.representedHE = nullptr; //reset selected mesh components

        // populate list widgets
        ui->mygl->my_mesh.setListWidgets(ui->vertsListWidget, ui->facesListWidget, ui->halfEdgesListWidget);
    }
}

//UPDATE CLICKED QLIST ITEMS
void MainWindow::on_vertsListWidget_itemClicked(QListWidgetItem *item) {
    ui->mygl->m_vertDisplay.updateVertex(dynamic_cast<Vertex*>(item));
}


void MainWindow::on_halfEdgesListWidget_itemClicked(QListWidgetItem *item) {
    ui->mygl->m_HEDisplay.updateHE(dynamic_cast<HalfEdge*>(item));
}


void MainWindow::on_facesListWidget_itemClicked(QListWidgetItem *item) {
    ui->mygl->m_faceDisplay.updateFace(dynamic_cast<Face*>(item));
}

//SLOTS TO CONNECT KEYPRESSEVENTS--delete
void MainWindow::updateHalfEdgeDisplay(HalfEdge* selectedHalfEdge) {}

void MainWindow::updateFaceDisplay(Face* selectedFace) {}

void MainWindow::updateVertexDisplay(Vertex* selectedVertex) {}


//SUBDIVISION BUTTONS
void MainWindow::on_splitEdge_clicked()
{
    ui->mygl->my_mesh.splitEdge(ui->mygl->m_HEDisplay.representedHE, ui->vertsListWidget, ui->halfEdgesListWidget);
    ui->mygl->m_HEDisplay.initializeAndBufferGeometryData(); //update HE display
    ui->mygl->m_vertDisplay.initializeAndBufferGeometryData(); //update vert display
}

void MainWindow::on_subdivide_clicked()
{
    ui->mygl->my_mesh.catmullClarkSubdivide(ui->vertsListWidget, ui->facesListWidget, ui->halfEdgesListWidget);
    ui->mygl->my_mesh.initializeAndBufferGeometryData();
    ui->mygl->m_HEDisplay.initializeAndBufferGeometryData(); //update HE display
    ui->mygl->m_vertDisplay.initializeAndBufferGeometryData(); //update vert display
    ui->mygl->m_faceDisplay.initializeAndBufferGeometryData(); //update face display

}

void MainWindow::on_pushButton_clicked() //to triangulate face
{
    ui->mygl->my_mesh.triangulateFace(ui->mygl->m_faceDisplay.representedFace, ui->facesListWidget, ui->halfEdgesListWidget);
    ui->mygl->my_mesh.initializeAndBufferGeometryData();
    ui->mygl->m_faceDisplay.initializeAndBufferGeometryData(); //update face display

}

//SPIN BOX SLOTS
void MainWindow::onVertexPositionChanged() {
    if (ui->mygl->m_vertDisplay.representedVertex != nullptr) {
        ui->mygl->m_vertDisplay.representedVertex->position.x = ui->vertPosXSpinBox->value();
        ui->mygl->m_vertDisplay.representedVertex->position.y = ui->vertPosYSpinBox->value();
        ui->mygl->m_vertDisplay.representedVertex->position.z = ui->vertPosZSpinBox->value();

        ui->mygl->my_mesh.initializeAndBufferGeometryData(); //update mesh drawing
        ui->mygl->m_vertDisplay.initializeAndBufferGeometryData(); //update vertex display
        update();
    }
}

void MainWindow::onFaceColorChanged() {
    if (ui->mygl->m_faceDisplay.representedFace != nullptr) {
        ui->mygl->m_faceDisplay.representedFace->color.r = ui->faceRedSpinBox->value();
        ui->mygl->m_faceDisplay.representedFace->color.g = ui->faceGreenSpinBox->value();
        ui->mygl->m_faceDisplay.representedFace->color.b = ui->faceBlueSpinBox->value();

        ui->mygl->my_mesh.initializeAndBufferGeometryData();
        update();
    }
}

