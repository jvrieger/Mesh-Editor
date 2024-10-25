#include "mainwindow.h"
#include <ui_mainwindow.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

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

//SLOTS TO CONNECT KEYPRESSEVENTS
void MainWindow::updateHalfEdgeDisplay(HalfEdge* selectedHalfEdge) {}

void MainWindow::updateFaceDisplay(Face* selectedFace) {}

void MainWindow::updateVertexDisplay(Vertex* selectedVertex) {}
