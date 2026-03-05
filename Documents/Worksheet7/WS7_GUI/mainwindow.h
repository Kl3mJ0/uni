#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ModelPart.h"
#include "ModelPartList.h"
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    ModelPartList* partList;
    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);

signals:
    void statusUpdateMessage(const QString& message, int timeout);

private slots:
    void handleAddButton();
    void handleOptionsButton();
    void handleTreeClicked();
    void on_actionOpen_File_triggered();
    void on_actionItem_Options_triggered();
};
#endif // MAINWINDOW_H
