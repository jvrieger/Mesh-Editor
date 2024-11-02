#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>
#include <QListWidgetItem>
#include "mesh.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateHalfEdgeDisplay(HalfEdge* selectedHalfEdge);
    void updateFaceDisplay(Face* selectedFace);
    void updateVertexDisplay(Vertex* selectedVertex);

private slots:
    void on_actionQuit_triggered();

    void on_openOBJ_clicked();

    void on_vertsListWidget_itemClicked(QListWidgetItem *item);

    void on_halfEdgesListWidget_itemClicked(QListWidgetItem *item);

    void on_facesListWidget_itemClicked(QListWidgetItem *item);

    void on_splitEdge_clicked();

    void on_subdivide_clicked();

    void on_pushButton_clicked();

    void onVertexPositionChanged();

    void onFaceColorChanged();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
